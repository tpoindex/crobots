#!/usr/bin/tclsh
 # Static Content Web Server Daemon
 # config is a global array containing the global server state
 #  root:    the root of the document directory
 #  port:    The port this server is serving
 #  listen:  the main listening socket id
 #  accepts: a count of accepted connections so far

 array set config {
   bufsize  32768
   sockblock  0
 }

 # HTTP/1.0 error codes (the ones we use)
 array set errors {
   204 {No Content}
   400 {Bad Request}
   404 {Not Found}
   503 {Service Unavailable}
   504 {Service Temporarily Unavailable}
 }

 # Start the server by listening for connections on the desired port.
 proc server {root { port 0 } { default "" } } {
   global config

   if { $port == 0 } { set port 8080 }
   if { "$default" == "" } { set default index.html }
   puts "Starting webserver, root at $root, port $port, default page $default"
   array set config [list root $root default $default]
   if {![info exists config(port)]} {
     set config(port) $port
     set config(listen) [socket -server accept_connect $port]
     set config(accepts) 0
   }
   return $config(port)
 }

 # Accept a new connection from the server and set up a handler
 # to read the request from the client.

 proc accept_connect {newsock ipaddr port} {
   global config
   upvar #0 config$newsock data

   incr config(accepts)
   fconfigure $newsock -blocking $config(sockblock) \
     -buffersize $config(bufsize) \
     -translation {auto crlf}
   putlog $newsock Connect $ipaddr $port
   set data(ipaddr) $ipaddr
   fileevent $newsock readable [list pull $newsock]
 }
 # read data from a client request
 proc pull { sock } {
   upvar #0 config$sock data

   set readCount [gets $sock line]
   if {![info exists data(state)]} {
     if [regexp {(POST|GET) ([^?]+)\??([^ ]*) HTTP/1.(0|1)} $line x data(proto) data(url) data(query)] {
       set data(state) mime
       putlog $sock Query $line
     } else {
       push-error $sock 400 "bad first line: $line"
     }
     return
   }

   set state [string compare $readCount 0],$data(state),$data(proto)
   switch -- $state {
     0,mime,GET  -
     0,query,POST  { push $sock }
     0,mime,POST   { set data(state) query }
     1,mime,POST   -
     1,mime,GET    {
       if [regexp {([^:]+):[   ]*(.*)}  $line dummy key value] {
         set data(mime,[string tolower $key]) $value
       }
     }
     1,query,POST  {
       set data(query) $line
       push $sock
     }
     default {
       if [eof $sock] {
         putlog $sock Error "unexpected eof on <$data(url)> request"
       } else {
         putlog $sock Error "unhandled state <$state> fetching <$data(url)>"
       }
       push-error $sock 404 ""
     }
   }
 }

 # Close a socket.
 proc disconnect { sock } {
   upvar #0 config$sock data
   unset data
   flush $sock
   close $sock
 }

 proc finish { mypath in out bytes { error {} } } {
   close $in
   disconnect $out
   putlog $out Done "$mypath"
 }

 # Respond to the query.
 proc push { sock } {
   global config
   upvar #0 config$sock data

   set mypath [ URLtoString "$config(root)$data(url)"]
   regsub -all "\\.\\./" $mypath "" mypath
   if {[file isdirectory $mypath]} { append mypath $config(default) }

   if {[string length $mypath] == 0} {
     push-error $sock 400 "$data(url) invalid path"
     return
   }

   if {![catch {open $mypath r} in]} {
     puts $sock "HTTP/1.0 200 Data follows"
     puts $sock "Date: [fmtdate [clock seconds]]"
     puts $sock "Last-Modified: [fmtdate [file mtime $mypath]]"
     puts $sock "Content-Type: [mime-type $mypath]"
     puts $sock "Content-Length: [file size $mypath]"
     puts $sock ""
     fconfigure $sock -translation binary -blocking $config(sockblock)
     fconfigure $in -translation binary -blocking 1
     fcopy $in $sock -command [list finish $mypath $in $sock]
   } else {
     push-error $sock 404 "$data(url) $in"
   }
 }

 # Convert the file suffix into a mime type
 array set mimetypes {
   {}    text/plain
   .txt  text/plain
   .htm  text/html
   .html text/html
   .gif  image/gif
   .jpg  image/jpeg
   .xbm  image/x-xbitmap
 }

 proc mime-type {path} {
   global mimetypes

   set type text/plain
   catch {set type $mimetypes([file extension $path])}
   return $type
 }

 proc push-error {sock code errmsg } {
   upvar #0 config$sock data
   global errors

   append data(url) ""
   set message "<title>Error: $code</title>Error <b>$data(url): $errors($code)</b>."
   puts $sock "HTTP/1.0 $code $errors($code)"
   puts $sock "Date: [fmtdate [clock seconds]]"
   puts $sock "Content-Length: [string length $message]"
   puts $sock ""
   puts $sock $message
   flush $sock
   putlog $sock Error $message
   disconnect $sock
 }

 # Generate a date string in HTTP format.
 proc fmtdate {seconds} {
   return [clock format $seconds -format {%a, %d %b %Y %T %Z}]
 }

 # Log a transaction.
 proc putlog {sock reason args} {
   puts "[clock format [clock seconds]]\t$sock\t$reason\t[join $args { }]"
 }

 # Decode url-encoded strings.
 proc URLtoString {data} {
   regsub -all {([][$\\])} $data {\\\1} data
   regsub -all {%([0-9a-fA-F][0-9a-fA-F])} $data  {[format %c 0x\1]} data
   return [subst $data]
 }

 proc bgerror {msg} {
     global errorInfo
     puts stderr "bgerror: $msg\n$errorInfo"
 }

 if { $argc < 1 } { puts "swsd <root directory> <port number> <default page name>"; exit }
 eval server $argv
 vwait forever    ;# start the Tcl event loop


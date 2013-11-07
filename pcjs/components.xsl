<?xml version="1.0" encoding="UTF-8"?>
<!-- author="Jeff Parsons (@jeffpar)" website="http://jsmachines.net/" created="2012-05-05" modified="2013-01-29" license="http://www.gnu.org/licenses/gpl.html" -->
<!DOCTYPE xsl:stylesheet [
	<!-- XSLT understands these entities only: lt, gt, apos, quot, and amp.  Other required entities may be defined below (see http://jsmachines.net/styles/common/entities.dtd). --> 
]>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:param name="rootDir" select="''"/>
	<xsl:param name="generator" select="'client'"/>

	<xsl:variable name="APPNAME">pcjs</xsl:variable>
	<xsl:variable name="APPVER">1.10</xsl:variable>
	<xsl:variable name="SITENAME">jsmachines</xsl:variable>
	<xsl:variable name="SITEDOMAIN">jsmachines.net</xsl:variable>

	<xsl:template name="componentStyles">
		<xsl:param name="component"></xsl:param>
		<xsl:choose>
			<xsl:when test="$component != ''">
				<link rel="stylesheet" type="text/css" href="/styles/pc/components/{$component}.css"/>
			</xsl:when>
			<xsl:otherwise>
				<link rel="stylesheet" type="text/css" href="/styles/pc/components/components.css"/>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>

	<xsl:template name="componentScripts">
		<xsl:param name="component"></xsl:param>
		<script type="text/javascript" src="{$component}.js"></script>
	</xsl:template>
	
	<xsl:template name="componentIncludes">
		<xsl:param name="component"></xsl:param>
		<xsl:call-template name="componentStyles"><xsl:with-param name="component" select="$component"/></xsl:call-template>
		<xsl:call-template name="componentScripts"><xsl:with-param name="component" select="$component"/></xsl:call-template>
	</xsl:template>
	
	<xsl:template match="machine[@ref]">
		<xsl:variable name="componentFile"><xsl:value-of select="$rootDir"/><xsl:value-of select="@ref"/></xsl:variable>
		<xsl:apply-templates select="document($componentFile)/machine"></xsl:apply-templates>
	</xsl:template>
	
	<xsl:template match="machine[not(@ref)]">
		<xsl:param name="machine"><xsl:value-of select="@id"/></xsl:param>
		<xsl:call-template name="component">
			<xsl:with-param name="machine" select="$machine"/>
			<xsl:with-param name="class" select="@class"/>
			<xsl:with-param name="parms"><xsl:if test="@parms">,<xsl:value-of select="@parms"/></xsl:if></xsl:with-param>
		</xsl:call-template>
	</xsl:template>
	
	<xsl:template match="component[@ref]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="component" select="name(.)"/>
		<xsl:variable name="componentFile"><xsl:value-of select="$rootDir"/><xsl:value-of select="@ref"/></xsl:variable>
		<xsl:apply-templates select="document($componentFile)/component"><xsl:with-param name="machine" select="$machine"/></xsl:apply-templates>
	</xsl:template>
	
	<xsl:template match="component[not(@ref)]">
		<xsl:param name="machine"></xsl:param>
		<xsl:call-template name="component">
			<xsl:with-param name="machine" select="$machine"/>
			<xsl:with-param name="class" select="@class"/>
			<xsl:with-param name="parms"><xsl:if test="@parms">,<xsl:value-of select="@parms"/></xsl:if></xsl:with-param>
		</xsl:call-template>
	</xsl:template>
	
	<xsl:template name="component">
		<xsl:param name="machine"/>
		<xsl:param name="component" select="name(.)"/>
		<xsl:param name="class"/>
		<xsl:param name="classWidth"/>
		<xsl:param name="classHeight"/>
		<xsl:param name="parms"/>
		<xsl:variable name="id">
			<xsl:choose>
				<xsl:when test="$component = 'machine'"><xsl:value-of select="$machine"/></xsl:when>
				<xsl:when test="$machine != '' and @id"><xsl:value-of select="$machine"/>.<xsl:value-of select="@id"/></xsl:when>
				<xsl:when test="$machine != ''"><xsl:value-of select="$machine"/>.<xsl:value-of select="$component"/></xsl:when>
				<xsl:when test="@id"><xsl:value-of select="@id"/></xsl:when>
				<xsl:otherwise><xsl:value-of select="$component"/></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="name">
			<xsl:choose>
				<xsl:when test="name"><xsl:value-of select="name"/></xsl:when>
				<xsl:when test="@name"><xsl:value-of select="@name"/></xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="comment">
			<xsl:choose>
				<xsl:when test="@comment">,comment:'<xsl:value-of select="@comment"/>'</xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="border">
			<xsl:choose>
				<xsl:when test="@border = '1'">border:1px solid black; overflow:auto; width:100%;</xsl:when>
				<xsl:when test="@border">border:<xsl:value-of select="@border"/>;</xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="width">
			<xsl:choose>
				<xsl:when test="@width">width:<xsl:value-of select="@width"/>;</xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="height">
			<xsl:choose>
				<xsl:when test="@height">height:<xsl:value-of select="@height"/>;</xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="left">
			<xsl:choose>
				<xsl:when test="@left">left:<xsl:value-of select="@left"/>;</xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="top">
			<xsl:choose>
				<xsl:when test="@top">top:<xsl:value-of select="@top"/>;</xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="padleft">
			<xsl:choose>
				<xsl:when test="@padleft">padding-left:<xsl:value-of select="@padleft"/>;</xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="padright">
			<xsl:choose>
				<xsl:when test="@padright">padding-right:<xsl:value-of select="@padright"/>;</xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="padtop">
			<xsl:choose>
				<xsl:when test="@padtop">padding-top:<xsl:value-of select="@padtop"/>;</xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="padbottom">
			<xsl:choose>
				<xsl:when test="@padbottom">padding-bottom:<xsl:value-of select="@padbottom"/>;</xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="pos">
			<xsl:choose>
				<xsl:when test="@pos = 'left'">float:left;</xsl:when>
				<xsl:when test="@pos = 'right'">float:right;</xsl:when>
				<xsl:when test="@pos = 'center'">margin:0 auto;</xsl:when>
				<xsl:when test="@pos">position:<xsl:value-of select="@pos"/>;</xsl:when>
				<xsl:when test="$left != '' or $top != ''">position:relative;</xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="style">
			<xsl:choose>
				<xsl:when test="@style"><xsl:value-of select="@style"/></xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="componentClass">
			<xsl:choose>
				<xsl:when test="$component = 'machine'"><xsl:value-of select="$SITENAME"/><xsl:text>-machine</xsl:text><xsl:if test="$class"><xsl:text> </xsl:text><xsl:value-of select="$class"/></xsl:if></xsl:when>
				<xsl:otherwise><xsl:value-of select="$SITENAME"/><xsl:text>-</xsl:text><xsl:value-of select="$component"/><xsl:text> </xsl:text><xsl:value-of select="$SITENAME"/><xsl:text>-component</xsl:text></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:if test="not(@pos)"><div style="clear:both"/></xsl:if>
		<div id="{$id}" class="{$componentClass}" style="{$width}{$height}{$pos}{$left}{$top}{$padleft}{$padright}{$padtop}{$padbottom}">
			<xsl:if test="$component = 'machine'">
				<xsl:apply-templates select="name" mode="machine"/>
			</xsl:if>
			<xsl:if test="$component != 'machine'">
				<xsl:apply-templates select="name" mode="component"/>
			</xsl:if>
			<div class="{$SITENAME}-container" style="{$border}{$style}">
				<xsl:if test="$class and $component != 'machine'">
					<div class="{$APPNAME}-{$class}" style="{$classWidth}{$classHeight}" data-value="id:'{$id}',name:'{$name}'{$comment}{$parms}"></div>
				</xsl:if>
				<xsl:if test="control">
					<div class="{$SITENAME}-controls">
						<xsl:apply-templates select="control" mode="component"/>
					</div>
				</xsl:if>
				<xsl:apply-templates><xsl:with-param name="machine" select="$machine"/></xsl:apply-templates>
			</div>
			<xsl:if test="$component = 'machine'">
				<div style="clear:both"/><div style="font-size:x-small; text-align:right"><a href="http://{$SITEDOMAIN}/{$APPNAME}" target="_blank">PCjs Simulator</a> v<xsl:value-of select="$APPVER"/> © 2012-2013 by <a href="http://twitter.com/jeffpar" target="_blank">@jeffpar</a></div>
			</xsl:if>
		</div>
	</xsl:template>

	<xsl:template match="name" mode="machine">
		<xsl:variable name="pos">
			<xsl:choose>
				<xsl:when test="@pos = 'center'">text-align:center;</xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<h2 style="{$pos}"><xsl:apply-templates/></h2>
	</xsl:template>
	
	<xsl:template match="name" mode="component">
		<div class="name"><xsl:apply-templates/></div>
	</xsl:template>
	
	<xsl:template match="control" mode="component">
		<xsl:variable name="type">
			<xsl:text>type:'</xsl:text><xsl:value-of select="@type"/><xsl:text>'</xsl:text>
		</xsl:variable>
		<xsl:variable name="binding">
			<xsl:text>binding:'</xsl:text><xsl:value-of select="@binding"/><xsl:text>'</xsl:text>
		</xsl:variable>
		<xsl:variable name="border">
			<xsl:choose>
				<xsl:when test="@border = '1'">border:1px solid black;</xsl:when>
				<xsl:when test="@border">border:<xsl:value-of select="@border"/>;</xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="width">
			<xsl:choose>
				<xsl:when test="@width">width:<xsl:value-of select="@width"/>;</xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="height">
			<xsl:choose>
				<xsl:when test="@height">height:<xsl:value-of select="@height"/>;</xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="left">
			<xsl:choose>
				<xsl:when test="@left">left:<xsl:value-of select="@left"/>;</xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="top">
			<xsl:choose>
				<xsl:when test="@top">top:<xsl:value-of select="@top"/>;</xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="padleft">
			<xsl:choose>
				<xsl:when test="@padleft">padding-left:<xsl:value-of select="@padleft"/>;</xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="padright">
			<xsl:choose>
				<xsl:when test="@padright">padding-right:<xsl:value-of select="@padright"/>;</xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="padtop">
			<xsl:choose>
				<xsl:when test="@padtop">padding-top:<xsl:value-of select="@padtop"/>;</xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="padbottom">
			<xsl:choose>
				<xsl:when test="@padbottom">padding-bottom:<xsl:value-of select="@padbottom"/>;</xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="pos">
			<xsl:choose>
				<xsl:when test="@pos = 'left'">float:left;</xsl:when>
				<xsl:when test="@pos = 'right'">float:right;</xsl:when>
				<xsl:when test="@pos = 'center'">margin:0 auto;</xsl:when>
				<xsl:when test="@pos">position:<xsl:value-of select="@pos"/>;</xsl:when>
				<xsl:when test="$left != '' or $top != ''">position:relative;</xsl:when>
				<xsl:otherwise><xsl:if test="$left = ''">float:left;</xsl:if></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="style">
			<xsl:choose>
				<xsl:when test="@style"><xsl:value-of select="@style"/></xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="containerStyle">
			<xsl:value-of select="$pos"/><xsl:value-of select="$left"/><xsl:value-of select="$top"/><xsl:value-of select="$padleft"/><xsl:value-of select="$padright"/><xsl:value-of select="$padtop"/><xsl:value-of select="$padbottom"/>
			<xsl:choose>
				<xsl:when test="@type = 'container'"><xsl:value-of select="$border"/><xsl:value-of select="$width"/><xsl:value-of select="$height"/><xsl:value-of select="$style"/></xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<div class="{$SITENAME}-control" style="{$containerStyle}">
			<xsl:variable name="fontsize">
				<xsl:choose>
					<xsl:when test="@size = 'large' or @size = 'small'">font-size:<xsl:value-of select="@size"/>;</xsl:when>
					<xsl:otherwise></xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:variable name="subclass">
				<xsl:if test="@label"><xsl:text> </xsl:text><xsl:value-of select="$SITENAME"/><xsl:text>-label</xsl:text></xsl:if>
			</xsl:variable>
			<xsl:variable name="labelwidth">
				<xsl:if test="@labelwidth">width:<xsl:value-of select="@labelwidth"/>;</xsl:if>
			</xsl:variable>
			<xsl:variable name="labelstyle">
				<xsl:choose>
					<xsl:when test="@labelstyle"><xsl:value-of select="@labelstyle"/></xsl:when>
					<xsl:otherwise>text-align:right;</xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:if test="@label">
				<xsl:if test="not(@labelpos) or @labelpos = 'left'">
					<div class="{$SITENAME}-label" style="{$labelwidth}{$labelstyle}"><xsl:value-of select="@label"/></div>
				</xsl:if>
			</xsl:if>
			<xsl:choose>
				<xsl:when test="@type = 'button'">
					<button class="{$SITENAME}-{@class}" style="-webkit-user-select:none;{$border}{$width}{$height}{$fontsize}{$style}" data-value="{$type},{$binding}"><xsl:apply-templates/></button>
				</xsl:when>
				<xsl:when test="@type = 'list'">
					<select class="{$SITENAME}-{@class}" style="{$border}{$width}{$height}{$fontsize}{$style}" data-value="{$type},{$binding}">
						<xsl:apply-templates select="disk|app" mode="component"/>
					</select>
				</xsl:when>
				<xsl:when test="@type = 'text'">
					<input class="{$SITENAME}-{@class}" type="text" style="{$border}{$width}{$height}{$style}" data-value="{$type},{$binding}" value="" autocapitalize="off" autocorrect="off"/>
				</xsl:when>
				<xsl:when test="@type = 'submit'">
					<input class="{$SITENAME}-{@class}" type="submit" style="{$border}{$fontsize}{$style}" data-value="{$type},{$binding}" value="{.}"/>
				</xsl:when>
				<xsl:when test="@type = 'textarea'">
					<textarea class="{$SITENAME}-{@class}" style="{$border}{$width}{$height}{$style}" data-value="{$type},{$binding}" readonly="readonly"></textarea>
				</xsl:when>
				<xsl:when test="@type = 'heading'">
					<div><xsl:value-of select="."/></div>
				</xsl:when>
				<xsl:when test="@type = 'separator'">
					<hr/>
				</xsl:when>
				<xsl:when test="@type = 'container'">
					<xsl:apply-templates mode="component"/>
				</xsl:when>
				<xsl:when test="not(@type)">
					<div style="clear:both"/><br/>
				</xsl:when>
				<xsl:otherwise>
					<div class="{$SITENAME}-{@class}{$subclass} {$SITENAME}-{@type}" style="-webkit-user-select:none;{$border}{$width}{$height}{$fontsize}{$style}" data-value="{$type},{$binding}"><xsl:apply-templates/></div>
				</xsl:otherwise>
			</xsl:choose>
			<xsl:if test="@label">
				<xsl:if test="@labelpos = 'right'">
					<div class="{$SITENAME}-label" style="{$labelwidth}{$labelstyle}"><xsl:value-of select="@label"/></div>
				</xsl:if>
				<div style="clear:both"/>
			</xsl:if>
		</div>
	</xsl:template>
	
	<xsl:template match="disk[@ref]" mode="component">
		<xsl:variable name="componentFile"><xsl:value-of select="$rootDir"/><xsl:value-of select="@ref"/></xsl:variable>
		<xsl:apply-templates select="document($componentFile)/disk" mode="component"></xsl:apply-templates>
	</xsl:template>
	
	<xsl:template match="disk[not(@ref)]" mode="component">
		<xsl:variable name="desc">
			<xsl:if test="@desc">
				<xsl:text>desc:'</xsl:text><xsl:value-of select="@desc"/><xsl:text>'</xsl:text>
				<xsl:if test="@href">
					<xsl:text>,href:'</xsl:text><xsl:value-of select="@href"/><xsl:text>'</xsl:text>
				</xsl:if>
			</xsl:if>
		</xsl:variable>
		<option value="{@path}" data-value="{$desc}"><xsl:if test="name"><xsl:value-of select="name"/></xsl:if><xsl:if test="not(name)"><xsl:value-of select="."/></xsl:if></option>
	</xsl:template>

	<xsl:template match="app[@ref]" mode="component">
		<xsl:variable name="componentFile"><xsl:value-of select="$rootDir"/><xsl:value-of select="@ref"/></xsl:variable>
		<xsl:apply-templates select="document($componentFile)/app" mode="component"/>
	</xsl:template>
	
	<xsl:template match="app[not(@ref)]" mode="component">
		<xsl:variable name="desc">
			<xsl:if test="@desc">
				<xsl:text>desc:'</xsl:text><xsl:value-of select="@desc"/><xsl:text>'</xsl:text>
				<xsl:if test="@href">
					<xsl:text>,href:'</xsl:text><xsl:value-of select="@href"/><xsl:text>'</xsl:text>
				</xsl:if>
			</xsl:if>
		</xsl:variable>
		<xsl:variable name="path">
			<xsl:if test="@path"><xsl:value-of select="@path"/></xsl:if>
		</xsl:variable>
		<xsl:variable name="files">
			<xsl:for-each select="file"><xsl:if test="position() = 1"><xsl:value-of select="$path"/></xsl:if><xsl:value-of select="@name"/><xsl:if test="position() != last()">;</xsl:if></xsl:for-each>
		</xsl:variable>
		<option value="{$files}" data-value="{$desc}"><xsl:value-of select="@name"/></option>
	</xsl:template>

	<xsl:template match="name">
	</xsl:template>
	
	<xsl:template match="control">
	</xsl:template>
	
	<xsl:template match="cpu[@ref]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="componentFile"><xsl:value-of select="$rootDir"/><xsl:value-of select="@ref"/></xsl:variable>
		<xsl:apply-templates select="document($componentFile)/cpu"><xsl:with-param name="machine" select="$machine"></xsl:with-param></xsl:apply-templates>
	</xsl:template>
	
	<xsl:template match="cpu[not(@ref)]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="model">
			<xsl:choose>
				<xsl:when test="@model"><xsl:value-of select="@model"/></xsl:when>
				<xsl:otherwise>8088</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="clock">
			<xsl:choose>
				<xsl:when test="@clock"><xsl:value-of select="@clock"/></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="multiplier">
			<xsl:choose>
				<xsl:when test="@multiplier"><xsl:value-of select="@multiplier"/></xsl:when>
				<xsl:otherwise>1</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="autoStart">
			<xsl:choose>
				<xsl:when test="@autostart"><xsl:value-of select="@autostart"/></xsl:when>
				<xsl:otherwise>null</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="csStart">
			<xsl:choose>
				<xsl:when test="@csstart"><xsl:value-of select="@csstart"/></xsl:when>
				<xsl:otherwise>-1</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="csInterval">
			<xsl:choose>
				<xsl:when test="@csinterval"><xsl:value-of select="@csinterval"/></xsl:when>
				<xsl:otherwise>-1</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="csStop">
			<xsl:choose>
				<xsl:when test="@csstop"><xsl:value-of select="@csstop"/></xsl:when>
				<xsl:otherwise>-1</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:call-template name="component">
			<xsl:with-param name="machine" select="$machine"/>
			<xsl:with-param name="class" select="'cpu'"/>
			<xsl:with-param name="parms">,model:<xsl:value-of select="$model"/>,clock:<xsl:value-of select="$clock"/>,multiplier:<xsl:value-of select="$multiplier"/>,autoStart:<xsl:value-of select="$autoStart"/>,autoStart:<xsl:value-of select="$autoStart"/>,csStart:<xsl:value-of select="$csStart"/>,csInterval:<xsl:value-of select="$csInterval"/>,csStop:<xsl:value-of select="$csStop"/></xsl:with-param>
		</xsl:call-template>
	</xsl:template>
	
	<xsl:template match="chipset[@ref]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="componentFile"><xsl:value-of select="$rootDir"/><xsl:value-of select="@ref"/></xsl:variable>
		<xsl:apply-templates select="document($componentFile)/chipset"><xsl:with-param name="machine" select="$machine"></xsl:with-param></xsl:apply-templates>
	</xsl:template>
	
	<xsl:template match="chipset[not(@ref)]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="model">
			<xsl:choose>
				<xsl:when test="@model"><xsl:value-of select="@model"/></xsl:when>
				<xsl:otherwise>5150</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="sw1">
			<xsl:choose>
				<xsl:when test="@sw1"><xsl:value-of select="@sw1"/></xsl:when>
				<xsl:otherwise>11110011</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="sw2">
			<xsl:choose>
				<xsl:when test="@sw2"><xsl:value-of select="@sw2"/></xsl:when>
				<xsl:otherwise>11110000</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:call-template name="component">
			<xsl:with-param name="machine" select="$machine"/>
			<xsl:with-param name="class">chipset</xsl:with-param>
			<xsl:with-param name="parms">,model:'<xsl:value-of select="$model"/>',sw1:'<xsl:value-of select="$sw1"/>',sw2:'<xsl:value-of select="$sw2"/>'</xsl:with-param>
		</xsl:call-template>
	</xsl:template>
	
	<xsl:template match="keyboard[@ref]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="componentFile"><xsl:value-of select="$rootDir"/><xsl:value-of select="@ref"/></xsl:variable>
		<xsl:apply-templates select="document($componentFile)/keyboard"><xsl:with-param name="machine" select="$machine"></xsl:with-param></xsl:apply-templates>
	</xsl:template>
	
	<xsl:template match="keyboard[not(@ref)]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="model">
			<xsl:choose>
				<xsl:when test="@model"><xsl:value-of select="@model"/></xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:call-template name="component">
			<xsl:with-param name="machine" select="$machine"/>
			<xsl:with-param name="class">keyboard</xsl:with-param>
			<xsl:with-param name="parms">,model:'<xsl:value-of select="$model"/>'</xsl:with-param>
		</xsl:call-template>
	</xsl:template>
	
	<xsl:template match="serial[@ref]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="componentFile"><xsl:value-of select="$rootDir"/><xsl:value-of select="@ref"/></xsl:variable>
		<xsl:apply-templates select="document($componentFile)/serial"><xsl:with-param name="machine" select="$machine"></xsl:with-param></xsl:apply-templates>
	</xsl:template>
	
	<xsl:template match="serial[not(@ref)]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="adapter">
			<xsl:choose>
				<xsl:when test="@adapter"><xsl:value-of select="@adapter"/></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="binding">
			<xsl:choose>
				<xsl:when test="@binding"><xsl:value-of select="@binding"/></xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:call-template name="component">
			<xsl:with-param name="machine" select="$machine"/>
			<xsl:with-param name="class">serial</xsl:with-param>
			<xsl:with-param name="parms">,adapter:<xsl:value-of select="$adapter"/>,binding:'<xsl:value-of select="$binding"/>'</xsl:with-param>
		</xsl:call-template>
	</xsl:template>
	
	<xsl:template match="mouse[@ref]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="componentFile"><xsl:value-of select="$rootDir"/><xsl:value-of select="@ref"/></xsl:variable>
		<xsl:apply-templates select="document($componentFile)/mouse"><xsl:with-param name="machine" select="$machine"></xsl:with-param></xsl:apply-templates>
	</xsl:template>
	
	<xsl:template match="mouse[not(@ref)]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="serial">
			<xsl:choose>
				<xsl:when test="@serial"><xsl:value-of select="@serial"/></xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:call-template name="component">
			<xsl:with-param name="machine" select="$machine"/>
			<xsl:with-param name="class">mouse</xsl:with-param>
			<xsl:with-param name="parms">,serial:'<xsl:value-of select="$serial"/>'</xsl:with-param>
		</xsl:call-template>
	</xsl:template>
	
	<xsl:template match="fdc[@ref]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="componentFile"><xsl:value-of select="$rootDir"/><xsl:value-of select="@ref"/></xsl:variable>
		<xsl:apply-templates select="document($componentFile)/fdc"><xsl:with-param name="machine" select="$machine"></xsl:with-param></xsl:apply-templates>
	</xsl:template>
	
	<xsl:template match="fdc[not(@ref)]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="autoMount">
			<xsl:choose>
				<xsl:when test="@automount"><xsl:value-of select="@automount"/></xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:call-template name="component">
			<xsl:with-param name="machine" select="$machine"/>
			<xsl:with-param name="class">fdc</xsl:with-param>
			<xsl:with-param name="parms">,autoMount:'<xsl:value-of select="$autoMount"/>'</xsl:with-param>
		</xsl:call-template>
	</xsl:template>
	
	<xsl:template match="hdc[@ref]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="componentFile"><xsl:value-of select="$rootDir"/><xsl:value-of select="@ref"/></xsl:variable>
		<xsl:apply-templates select="document($componentFile)/hdc"><xsl:with-param name="machine" select="$machine"></xsl:with-param></xsl:apply-templates>
	</xsl:template>
	
	<xsl:template match="hdc[not(@ref)]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="drives">
			<xsl:choose>
				<xsl:when test="@drives"><xsl:value-of select="@drives"/></xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:call-template name="component">
			<xsl:with-param name="machine" select="$machine"/>
			<xsl:with-param name="class">hdc</xsl:with-param>
			<xsl:with-param name="parms">,drives:'<xsl:value-of select="$drives"/>'</xsl:with-param>
		</xsl:call-template>
	</xsl:template>
	
	<xsl:template match="rom[@ref]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="componentFile"><xsl:value-of select="$rootDir"/><xsl:value-of select="@ref"/></xsl:variable>
		<xsl:apply-templates select="document($componentFile)/rom"><xsl:with-param name="machine" select="$machine"></xsl:with-param></xsl:apply-templates>
	</xsl:template>
	
	<xsl:template match="rom[not(@ref)]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="addr">
			<xsl:choose>
				<xsl:when test="@addr"><xsl:value-of select="@addr"/></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="size">
			<xsl:choose>
				<xsl:when test="@size"><xsl:value-of select="@size"/></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="file">
			<xsl:choose>
				<xsl:when test="@file"><xsl:value-of select="@file"/></xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="notify">
			<xsl:choose>
				<xsl:when test="@notify"><xsl:value-of select="@notify"/></xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:call-template name="component">
			<xsl:with-param name="machine" select="$machine"/>
			<xsl:with-param name="class">rom</xsl:with-param>
			<xsl:with-param name="parms">,addr:<xsl:value-of select="$addr"/>,size:<xsl:value-of select="$size"/>,file:'<xsl:value-of select="$file"/>',notify:'<xsl:value-of select="$notify"/>'</xsl:with-param>
		</xsl:call-template>
	</xsl:template>
	
	<xsl:template match="ram[@ref]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="componentFile"><xsl:value-of select="$rootDir"/><xsl:value-of select="@ref"/></xsl:variable>
		<xsl:apply-templates select="document($componentFile)/ram"><xsl:with-param name="machine" select="$machine"></xsl:with-param></xsl:apply-templates>
	</xsl:template>
	
	<xsl:template match="ram[not(@ref)]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="addr">
			<xsl:choose>
				<xsl:when test="@addr"><xsl:value-of select="@addr"/></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="size">
			<xsl:choose>
				<xsl:when test="@size"><xsl:value-of select="@size"/></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="test">
			<xsl:choose>
				<xsl:when test="@test"><xsl:value-of select="@test"/></xsl:when>
				<xsl:otherwise>true</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:call-template name="component">
			<xsl:with-param name="machine" select="$machine"/>
			<xsl:with-param name="class">ram</xsl:with-param>
			<xsl:with-param name="parms">,addr:<xsl:value-of select="$addr"/>,size:<xsl:value-of select="$size"/>,test:<xsl:value-of select="$test"/></xsl:with-param>
		</xsl:call-template>
	</xsl:template>
	
	<xsl:template match="video[@ref]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="componentFile"><xsl:value-of select="$rootDir"/><xsl:value-of select="@ref"/></xsl:variable>
		<xsl:apply-templates select="document($componentFile)/video"><xsl:with-param name="machine" select="$machine"></xsl:with-param></xsl:apply-templates>
	</xsl:template>
	
	<xsl:template match="video[not(@ref)]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="model">
			<xsl:choose>
				<xsl:when test="@model"><xsl:value-of select="@model"/></xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="mode">
			<xsl:choose>
				<xsl:when test="@mode"><xsl:value-of select="@mode"/></xsl:when>
				<xsl:otherwise>7</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="screenWidth">
			<xsl:choose>
				<xsl:when test="@screenwidth"><xsl:value-of select="@screenwidth"/></xsl:when>
				<xsl:otherwise>256</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="screenHeight">
			<xsl:choose>
				<xsl:when test="@screenheight"><xsl:value-of select="@screenheight"/></xsl:when>
				<xsl:otherwise>224</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="scale">
			<xsl:choose>
				<xsl:when test="@scale"><xsl:value-of select="@scale"/></xsl:when>
				<xsl:otherwise>false</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="charCols">
			<xsl:choose>
				<xsl:when test="@cols"><xsl:value-of select="@cols"/></xsl:when>
				<xsl:otherwise>80</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="charRows">
			<xsl:choose>
				<xsl:when test="@rows"><xsl:value-of select="@rows"/></xsl:when>
				<xsl:otherwise>25</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="fontROM">
			<xsl:choose>
				<xsl:when test="@charset"><xsl:value-of select="@charset"/></xsl:when>
				<xsl:when test="@fontrom"><xsl:value-of select="@fontrom"/></xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="screenColor">
			<xsl:choose>
				<xsl:when test="@screencolor"><xsl:value-of select="@screencolor"/></xsl:when>
				<xsl:otherwise>black</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:call-template name="component">
			<xsl:with-param name="machine" select="$machine"/>
			<xsl:with-param name="class">video</xsl:with-param>
			<xsl:with-param name="classWidth">
				<xsl:choose>
					<xsl:when test="$screenWidth">width:<xsl:value-of select="$screenWidth"/>px;</xsl:when>
					<xsl:otherwise></xsl:otherwise>
				</xsl:choose>
			</xsl:with-param>
			<xsl:with-param name="classHeight">
				<xsl:choose>
					<xsl:when test="$screenHeight">height:<xsl:value-of select="$screenHeight"/>px;</xsl:when>
					<xsl:otherwise></xsl:otherwise>
				</xsl:choose>
			</xsl:with-param>
			<xsl:with-param name="parms">,model:'<xsl:value-of select="$model"/>',mode:<xsl:value-of select="$mode"/>,screenWidth:<xsl:value-of select="$screenWidth"/>,screenHeight:<xsl:value-of select="$screenHeight"/>,scale:<xsl:value-of select="$scale"/>,charCols:<xsl:value-of select="$charCols"/>,charRows:<xsl:value-of select="$charRows"/>,fontROM:'<xsl:value-of select="$fontROM"/>',screenColor:'<xsl:value-of select="$screenColor"/>'</xsl:with-param>
		</xsl:call-template>
	</xsl:template>
	
	<xsl:template match="debugger[@ref]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="componentFile"><xsl:value-of select="$rootDir"/><xsl:value-of select="@ref"/></xsl:variable>
		<xsl:apply-templates select="document($componentFile)/debugger"><xsl:with-param name="machine" select="$machine"></xsl:with-param></xsl:apply-templates>
	</xsl:template>
	
	<xsl:template match="debugger[not(@ref)]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="messages">
			<xsl:choose>
				<xsl:when test="@messages"><xsl:value-of select="@messages"/></xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:call-template name="component">
			<xsl:with-param name="machine" select="$machine"/>
			<xsl:with-param name="class">debugger</xsl:with-param>
			<xsl:with-param name="parms">,messages:'<xsl:value-of select="$messages"/>'</xsl:with-param>
		</xsl:call-template>
	</xsl:template>
	
	<xsl:template match="panel[@ref]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="componentFile"><xsl:value-of select="$rootDir"/><xsl:value-of select="@ref"/></xsl:variable>
		<xsl:apply-templates select="document($componentFile)/panel"><xsl:with-param name="machine" select="$machine"></xsl:with-param></xsl:apply-templates>
	</xsl:template>
	
	<xsl:template match="panel[not(@ref)]">
		<xsl:param name="machine"></xsl:param>
		<xsl:call-template name="component">
			<xsl:with-param name="machine" select="$machine"/>
			<xsl:with-param name="class">panel</xsl:with-param>
		</xsl:call-template>
	</xsl:template>
	
	<xsl:template match="computer[@ref]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="componentFile"><xsl:value-of select="$rootDir"/><xsl:value-of select="@ref"/></xsl:variable>
		<xsl:apply-templates select="document($componentFile)/computer"><xsl:with-param name="machine" select="$machine"></xsl:with-param></xsl:apply-templates>
	</xsl:template>
	
	<xsl:template match="computer[not(@ref)]">
		<xsl:param name="machine"></xsl:param>
		<xsl:variable name="resume">
			<xsl:choose>
				<xsl:when test="@resume"><xsl:value-of select="@resume"/></xsl:when>
				<xsl:otherwise>0</xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:variable name="state">
			<xsl:choose>
				<xsl:when test="@state"><xsl:value-of select="@state"/></xsl:when>
				<xsl:otherwise></xsl:otherwise>
			</xsl:choose>
		</xsl:variable>
		<xsl:call-template name="component">
			<xsl:with-param name="machine" select="$machine"/>
			<xsl:with-param name="class">computer</xsl:with-param>
			<xsl:with-param name="parms">,resume:'<xsl:value-of select="$resume"/>',state:'<xsl:value-of select="$state"/>'</xsl:with-param>
		</xsl:call-template>
	</xsl:template>
	
</xsl:stylesheet>

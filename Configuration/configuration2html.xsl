<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:c="urn:strike-devices:configuration"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xmlns="http://www.w3.org/1999/xhtml">
  
  <xsl:output method="xml" version="1.0" encoding="utf-8" indent="no"
    media-type="text/html" omit-xml-declaration="yes"
    doctype-public="-//W3C//DTD XHTML 1.0 Strict//EN"
    doctype-system="http://www.w3.org/2002/08/xhtml/xhtml1-strict.dtd" />
  
  <xsl:variable name="language" select="/c:configuration/@lang"/>
  <xsl:template match="c:configuration">
<html lang="{$language}" xml:lang="{$language}">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title><xsl:value-of select="c:title[@language = $language]"/></title>
    <!--
    <link rel="stylesheet" media="all" href="FILENAME.css" type="text/css" charset="utf-8" />
    <script src="FILENAME.js" type="text/javascript" charset="utf-8" />
    <script src="FILENAME.js" type="application/x-javascript" charset="utf-8" />
    -->
</head>
<body>
<xsl:apply-templates select="c:title | c:help">
  <xsl:sort select="@lang"/>
</xsl:apply-templates>
<xsl:apply-templates select="c:page"/>
</body>
</html>
  </xsl:template>

  <xsl:template match="c:title">
    <p style="font-size: 110%">
      <em>
        <xsl:text>Language:</xsl:text>
        <xsl:choose> 
          <xsl:when test="@lang">
            <xsl:value-of select="@lang"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="$language"/>
          </xsl:otherwise>
        </xsl:choose>
      </em>
    </p>
    <xsl:element name="h{count(parent::c:*)}">
      <xsl:value-of select="."/>
    </xsl:element>
  </xsl:template>
  
  <xsl:template match="c:help">
    <xsl:choose> 
      <xsl:when test="@href">
        <xsl:apply-templates select="document(@href)/html/node()" mode="import"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="node()" disable-output-escaping="yes"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  <xsl:template match="*" mode="import" priority="1">
    <xsl:element name="{local-name(.)}">
      <xsl:apply-templates select="@*" mode="import"/>
      <xsl:apply-templates select="node()" mode="import"/>
    </xsl:element>
  </xsl:template>
  
  <xsl:template match="@* | node()" mode="import">
    <xsl:copy>
      <xsl:apply-templates select="node()" mode="import"/>
    </xsl:copy>
  </xsl:template>
  
  <xsl:template match="c:page">
    <div style="border:1px solid red">
      <xsl:apply-templates select="c:boolean | c:choice"/>
    </div>
  </xsl:template>
  
  <xsl:template match="c:boolean">
    <div style="border:1px solid black">
      <h>Boolean</h>
      <xsl:apply-templates select="c:title | c:help">
        <xsl:sort select="@lang"/>
      </xsl:apply-templates>
    </div>
  </xsl:template>
  
  <xsl:template match="c:choice">
    <div style="border:1px solid black">
      <h>Choice</h>
      <xsl:apply-templates select="c:title | c:help">
        <xsl:sort select="@lang"/>
      </xsl:apply-templates>
      <ul>
        <xsl:apply-templates select="c:option"/>
      </ul>
    </div>
  </xsl:template>
  
  <xsl:template match="c:option">
    <li>
      <xsl:apply-templates select="c:title | c:help">
        <xsl:sort select="@lang"/>
      </xsl:apply-templates>
    </li>
  </xsl:template>
</xsl:stylesheet>
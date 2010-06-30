<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:c="urn:dualstrike:configuration">
  <xsl:output method="text" encoding="ascii"/>

	<xsl:template match="/">
		<xsl:apply-templates select="c:configuration"/>
	</xsl:template>
  
  <xsl:template match="c:configuration">
    <!-- TODO -->
  </xsl:template>
  
  <xsl:template name="define">
    <xsl:param name="name"/>
    <xsl:param name="value"/>
    <xsl:text>#define </xsl:text>
    <xsl:value-of select="$name"/>
    <xsl:text> </xsl:text>
    <xsl:value-of select="$value"/>
    <xsl:text>&#xa;</xsl:text>
  </xsl:template>
  
  <xsl:template name="shift-left">
    <xsl:param name="value"/>
    <xsl:param name="count"/>
    <xsl:text>(</xsl:text>
    <xsl:value-of select="$value"/>
    <xsl:text>&lt;&lt;</xsl:text>
    <xsl:value-of select="$count"/>
    <xsl:text>)</xsl:text>
  </xsl:template>
  
  <xsl:template name="array-accessor">
    <xsl:param name="array"/>
    <xsl:param name="array-index"/>
    <xsl:value-of select="$array"/>
    <xsl:text>[</xsl:text>
    <xsl:value-of select="array-index"/>
    <xsl:text>]</xsl:text>
  </xsl:template>
  
  <xsl:template name="set-array-bit">
    <xsl:param name="array"/>
    <xsl:param name="array-index"/>
    <xsl:param name="value"/>
    <xsl:param name="bit-index"/>
    
    <xsl:call-template name="array-accessor">
      <xsl:with-param name="array" select="$array"/>
      <xsl:with-param name="array-index" select="$array-index"/>
    </xsl:call-template>
    
    <xsl:text> </xsl:text>
    
    <xsl:choose>
      <xsl:when test="boolean($value)">|= </xsl:when>
      <xsl:otherwise>&amp;= ^</xsl:otherwise>
    </xsl:choose>
    
    <xsl:call-template name="shift-left">
      <xsl:with-param name="value" select="1"/>
      <xsl:with-param name="count" select="$bit-index"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="set-array-bits">
    <xsl:param name="array"/>
    <xsl:param name="array-index"/>
    <xsl:param name="value"/>
    <xsl:param name="bit-index"/>

    <xsl:variable name="value-text">
      <xsl:call-template name="shift-left">
        <xsl:with-param name="value" select="$value"/>
        <xsl:with-param name="count" select="$bit-index"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:variable name="array-accessor">
      <xsl:call-template name="array-accessor">
        <xsl:with-param name="array" select="$array"/>
        <xsl:with-param name="array-index" select="$array-index"/>
      </xsl:call-template>
    </xsl:variable>

    <xsl:value-of select="$array-accessor"/>    
    <xsl:text> |= </xsl:text>
    <xsl:value-of select="$value-text"/>
    <xsl:text>; </xsl:text>
    <xsl:value-of select="$array-accessor"/>    
    <xsl:text> &amp;= ^</xsl:text>
    <xsl:value-of select="$value-text"/>
    <xsl:text>;</xsl:text>
  </xsl:template>
</xsl:stylesheet>
<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:c="urn:dualstrike:configuration">
  <xsl:output method="text" encoding="ascii"/>

	<xsl:template match="/">
		<xsl:apply-templates select="node()"/>
	</xsl:template>
  
  <xsl:template match="c:configuration">
    <xsl:apply-templates select="@*"/>
    <xsl:attribute name="byte-dividers">
      <!-- TODO -->
    </xsl:attribute>
    <xsl:apply-templates select="node()"/>
  </xsl:template>

  <xsl:template match="c:choice">
    <xsl:apply-templates select="@*"/>
    <xsl:attribute name="bit-width">
      <!-- TODO -->
    </xsl:attribute>
    <xsl:apply-templates select="node()"/>
  </xsl:template>

  <xsl:template match="c:option">
    <xsl:apply-templates select="@*"/>
    <xsl:attribute name="bit-pattern">
      <!-- TODO -->
    </xsl:attribute>
    <xsl:apply-templates select="node()"/>
  </xsl:template>
  
  <xsl:template match="node()">
    <xsl:copy>
      <xsl:apply-templates select="@*"/>
      <xsl:apply-templates select="node()"/>
    </xsl:copy>
  </xsl:template>
</xsl:stylesheet>
<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:c="urn:strike-devices:configuration">
  <xsl:output method="text" encoding="ascii"/>

	<xsl:template match="/">
		<xsl:apply-templates select="c:configuration"/>
	</xsl:template>
  
  <xsl:template match="c:configuration">
    <xsl:text>
#ifndef __configuration_h_included__
#define __configuration_h_included__
/*
 * GENERATED COFIGURATION HEADER FILE
 * Title:   </xsl:text><xsl:value-of select="c:title[not(@lang) or @lang = current()/@lang][1]"/><xsl:text>
 * Device:  </xsl:text><xsl:value-of select="c:device"/><xsl:text>
 * Version: </xsl:text><xsl:value-of select="c:version"/><xsl:text>
 */

#define </xsl:text><xsl:value-of select="@prefix"/><xsl:text>BYTE_WIDTH </xsl:text><xsl:value-of select="@byte-width"/><xsl:text>
#define EEPROM_DEF 0xFF

/* 
 * IMPORTANT:
 * Use must use the following macro in your main module to declare the variables
 * "config" and "config_EEPROM". 
 */  
#define </xsl:text>
  <xsl:value-of select="@prefix"/>
  <xsl:text>DECLARATION uint8_t config_EEPROM[</xsl:text>
  <xsl:value-of select="@byte-count"/>
  <xsl:text>] EEMEM = {</xsl:text>
  <!-- TODO -->
  <xsl:text>}; uint8_t config[</xsl:text>
  <xsl:value-of select="@byte-count"/>
  <xsl:text>] = {</xsl:text>
  <!-- TODO -->
  <xsl:text>};

</xsl:text>
  <xsl:apply-templates select="c:page/c:choice/c:option | c:page/c:boolean"/>
  <xsl:text>
#endif
</xsl:text>
  </xsl:template>
  
  <xsl:template match="c:boolean">
    <xsl:text>#define </xsl:text>
    <xsl:value-of select="/c:configuration/@prefix"/>
    <xsl:value-of select="@id"/>
    <xsl:text> (config[</xsl:text>
    <xsl:value-of select="@byte-no"/>
    <xsl:text>] &amp; (1&lt;&lt;</xsl:text>
    <xsl:value-of select="@bit-no"/>
    <xsl:text>))&#xA;</xsl:text>
  </xsl:template>

  <xsl:template match="c:option">
    <xsl:text>#define </xsl:text>
    <xsl:value-of select="/c:configuration/@prefix"/>
    <xsl:value-of select="SET_"/>
    <xsl:value-of select="parent::c:choice/@prefix"/>
    <xsl:value-of select="@id"/>
    <!-- TODO -->
    <xsl:text> (config[</xsl:text>
    <xsl:value-of select="parent::c:choice/@byte-no"/>
    <xsl:text>] &amp; (0b</xsl:text>
    <xsl:value-of select="@bit-pattern"/>
    <xsl:text>&lt;&lt;</xsl:text>
    <xsl:value-of select="parent::c:choice/@bit-no"/>
    <xsl:text>))&#xA;</xsl:text>
  </xsl:template>
</xsl:stylesheet>
<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:c="urn:strike-devices:configuration">
  <xsl:output method="text" encoding="ascii"/>

	<xsl:template match="/">
		<xsl:apply-templates select="c:configuration"/>
	</xsl:template>
  
  <xsl:template match="c:configuration">
    <xsl:text>/*
 * GENERATED CONFIGURATION HEADER FILE
 * Title:   </xsl:text><xsl:value-of select="c:title[not(@lang) or @lang = current()/@lang][1]"/><xsl:text>
 * Device:  </xsl:text><xsl:value-of select="c:device"/><xsl:text>
 * Version: </xsl:text><xsl:value-of select="c:version"/><xsl:text>
 */
 
#ifndef __configuration_h_included__
#define __configuration_h_included__

#include &lt;avr/eeprom.h> /* EEPROM functions */
#include &lt;stdint.h> /* C99 typedefs */ 

#define CONFIG_DEVICE (uint8_t)</xsl:text><xsl:value-of select="c:device"/><xsl:text>
#define CONFIG_VERSION (uint8_t)</xsl:text><xsl:value-of select="c:version"/><xsl:text>
#define CONFIG_BYTE_WIDTH </xsl:text><xsl:value-of select="@byte-width"/><xsl:text>
#define CONFIG_EMPTY 0xFF</xsl:text>
  <xsl:call-template name="create-config-defaults">
    <xsl:with-param name="default-bits">
      <xsl:apply-templates select="(c:page/c:choice | c:page/c:boolean)[1]" mode="default-bits"/>
    </xsl:with-param>
    <xsl:with-param name="byte-count" select="@byte-width"/>
  </xsl:call-template>
<xsl:text>

/* 
 * IMPORTANT:
 * You must use the following macro in your main module to declare the variables
 * "config" and "config_EEPROM". 
 */  
#define </xsl:text>
  <xsl:value-of select="@prefix"/>
  <xsl:text>DECLARATION \
uint8_t config_EEPROM[CONFIG_BYTE_WIDTH + 2] EEMEM = {CONFIG_DEVICE, CONFIG_VERSION, </xsl:text>
  <xsl:call-template name="create-list">
    <xsl:with-param name="prefix">CONFIG_DEF_</xsl:with-param>
    <xsl:with-param name="count" select="@byte-width"/>
  </xsl:call-template>
  <xsl:text>}; \
uint8_t config[CONFIG_BYTE_WIDTH + 2] = {</xsl:text>
  <xsl:call-template name="create-list">
    <xsl:with-param name="prefix">CONFIG_EMPTY</xsl:with-param>
    <xsl:with-param name="add-count" select="false()"/>
    <xsl:with-param name="count" select="@byte-width + 2"/>
  </xsl:call-template>
  <xsl:text>};

/*
 * Use the following macro to access the configuration from non-main-modules:
 */
#define </xsl:text>
  <xsl:value-of select="@prefix"/>
  <xsl:text>EXTERN_DECLARATION extern uint8_t config[CONFIG_BYTE_WIDTH + 2];
  
/* CONFIG TESTS: */
</xsl:text>
  <xsl:apply-templates select="c:page/c:choice/c:option | c:page/c:boolean" mode="create-tests"/>
  <xsl:text>
/* CONFIG SETTERS: */
</xsl:text>
  <xsl:apply-templates select="c:page/c:choice/c:option | c:page/c:boolean" mode="create-setters"/>
  <xsl:text>
#endif
</xsl:text>
  </xsl:template>
  
  <xsl:template match="c:boolean" mode="default-bits">
    <xsl:apply-templates select="(following::c:boolean | following::c:choice)[1]" mode="default-bits"/>
    <xsl:choose>
      <xsl:when test="@default = 'true'">1</xsl:when>
      <xsl:otherwise>0</xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="c:choice" mode="default-bits">
    <xsl:apply-templates select="(following::c:boolean | following::c:choice)[1]" mode="default-bits"/>
    <xsl:value-of select="c:option[@id = current()/@default]/@bit-pattern"/>
  </xsl:template>
  
  <xsl:template name="create-config-defaults">
    <xsl:param name="default-bits"/>
    <xsl:param name="byte-count"/>
    <xsl:param name="byte-index" select="0"/>
    <xsl:if test="$byte-index &lt; $byte-count">
      <xsl:text>
#define CONFIG_DEF_</xsl:text><xsl:value-of select="$byte-index + 1"/><xsl:text> 0b</xsl:text>
        <xsl:value-of select="substring($default-bits, string-length($default-bits) - (($byte-index + 1) * 7) + 1, 7)"/>
        <xsl:text>;</xsl:text>
        <xsl:call-template name="create-config-defaults">
          <xsl:with-param name="default-bits" select="$default-bits"/>
          <xsl:with-param name="byte-count" select="$byte-count"/>
          <xsl:with-param name="byte-index" select="$byte-index + 1"/>
        </xsl:call-template>
    </xsl:if>
  </xsl:template>
  
  <xsl:template name="create-list">
    <xsl:param name="prefix"></xsl:param>
    <xsl:param name="add-count" select="true()"/>
    <xsl:param name="count"/>
    <xsl:if test="$count > 1">
      <xsl:call-template name="create-list">
        <xsl:with-param name="prefix" select="$prefix"/>
        <xsl:with-param name="add-count" select="$add-count"/>
        <xsl:with-param name="count" select="$count - 1"/>
      </xsl:call-template>
      <xsl:text>, </xsl:text>
    </xsl:if>
    <xsl:value-of select="$prefix"/>
    <xsl:if test="$add-count">
      <xsl:value-of select="$count"/>
    </xsl:if>
  </xsl:template>
  
  <xsl:template match="c:boolean" mode="create-tests">
    <xsl:text>#define </xsl:text>
    <xsl:value-of select="/c:configuration/@prefix"/>
    <xsl:value-of select="@id"/>
    <xsl:text> (config[</xsl:text>
    <xsl:value-of select="@byte-no"/>
    <xsl:text> + 2] &amp; (1&lt;&lt;</xsl:text>
    <xsl:value-of select="@bit-no"/>
    <xsl:text>))&#xA;</xsl:text>
  </xsl:template>

  <xsl:template match="c:option" mode="create-tests">
    <xsl:text>#define </xsl:text>
    <xsl:value-of select="/c:configuration/@prefix"/>
    <xsl:value-of select="parent::c:choice/@prefix"/>
    <xsl:value-of select="@id"/>
    <xsl:text> </xsl:text>
    <xsl:if test="parent::c:choice/@byte-dividers">(</xsl:if>
    <xsl:apply-templates select="." mode="create-option-test"/>
    <xsl:if test="parent::c:choice/@byte-dividers">)</xsl:if>
    <xsl:text>&#xA;</xsl:text>
  </xsl:template>
  
  <xsl:template match="c:option" mode="create-option-test">
    <xsl:param name="dividers" select="parent::c:choice/@byte-dividers"/>
    <xsl:param name="bits" select="@bit-pattern"/>
    <xsl:param name="byte-no" select="parent::c:choice/@byte-no"/>
    <xsl:param name="bit-no" select="parent::c:choice/@bit-no"/>
    <xsl:choose>
      <xsl:when test="string-length($dividers) = 0">
        <xsl:text>(((config[</xsl:text>
        <xsl:value-of select="$byte-no"/>
        <xsl:text> + 2] >> </xsl:text>
        <xsl:value-of select="$bit-no"/>
        <xsl:text>) &amp; 0b</xsl:text>
        <xsl:call-template name="create-byte-mask">
          <xsl:with-param name="bits" select="string-length($bits)"/>
        </xsl:call-template>
        <xsl:text>)  == 0b</xsl:text>
        <xsl:value-of select="$bits"/>
        <xsl:text>)</xsl:text>
      </xsl:when>
      <xsl:otherwise>
        <xsl:variable name="divider">
          <xsl:choose>
            <xsl:when test="contains($dividers, ' ')">
              <xsl:value-of select="substring-before($dividers, ' ')"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$dividers"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:variable name="rest-dividers" select="substring-after($dividers, ' ')"/>
        <xsl:apply-templates select="." mode="create-option-test">
          <xsl:with-param name="dividers"></xsl:with-param>
          <xsl:with-param name="bits" select="substring($bits, 1, $divider)"/>
          <xsl:with-param name="bit-no" select="$bit-no"/>
          <xsl:with-param name="byte-no" select="$byte-no"/>
        </xsl:apply-templates>
        <xsl:variable name="rest-bits" select="substring($bits, $divider + 1)"/>
        <xsl:if test="$rest-bits != ''">
          <xsl:text> &amp;&amp; </xsl:text>
          <xsl:apply-templates select="." mode="create-option-test">
            <xsl:with-param name="dividers" select="$rest-dividers"/>
            <xsl:with-param name="byte-no" select="$byte-no + 1"/>
            <xsl:with-param name="bit-no" select="0"/>
            <xsl:with-param name="bits" select="$rest-bits"/>
          </xsl:apply-templates>
        </xsl:if>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="c:boolean" mode="create-setters">
    <xsl:text>#define </xsl:text>
    <xsl:value-of select="/c:configuration/@prefix"/>
    <xsl:text>ENABLE_</xsl:text>
    <xsl:value-of select="@id"/>
    <xsl:text>(CONFIG) CONFIG[</xsl:text>
    <xsl:value-of select="@byte-no"/>
    <xsl:text> + 2] |= (1&lt;&lt;</xsl:text>
    <xsl:value-of select="@bit-no"/>
    <xsl:text>);
#define </xsl:text>
    <xsl:value-of select="/c:configuration/@prefix"/>
    <xsl:text>DISABLE_</xsl:text>
    <xsl:value-of select="@id"/>
    <xsl:text>(CONFIG) CONFIG[</xsl:text>
    <xsl:value-of select="@byte-no"/>
    <xsl:text> + 2] &amp;= ~(1&lt;&lt;</xsl:text>
    <xsl:value-of select="@bit-no"/>
    <xsl:text>);&#xA;</xsl:text>
  </xsl:template>

  <xsl:template match="c:option" mode="create-setters">
    <xsl:apply-templates select="." mode="create-option-setters"/>
    <xsl:text>&#xA;</xsl:text>
  </xsl:template>
  
  <xsl:template match="c:option" mode="create-option-setters">
    <xsl:param name="dividers" select="parent::c:choice/@byte-dividers"/>
    <xsl:param name="bits" select="@bit-pattern"/>
    <xsl:param name="byte-no" select="parent::c:choice/@byte-no"/>
    <xsl:param name="bit-no" select="parent::c:choice/@bit-no"/>
    <xsl:choose>
      <xsl:when test="string-length($dividers) = 0">
        <xsl:if test="$byte-no = parent::c:choice/@byte-no">
          <xsl:text>#define </xsl:text>
          <xsl:value-of select="/c:configuration/@prefix"/>
          <xsl:text>SET_</xsl:text>
          <xsl:value-of select="parent::c:choice/@prefix"/>
          <xsl:value-of select="@id"/>
          <xsl:text> </xsl:text>
        </xsl:if>
        <xsl:choose>
          <xsl:when test="contains($bits, '1') and not(contains($bits, '0')) or contains($bits, '0') and not(contains($bits, '1'))">
            <xsl:text>(config[</xsl:text>
            <xsl:value-of select="$byte-no"/>
            <xsl:text> + 2] </xsl:text>
            <xsl:choose>
              <xsl:when test="contains($bits, '1')">|= </xsl:when>
              <xsl:otherwise>&amp;= ~</xsl:otherwise>
            </xsl:choose>
            <xsl:text>(0b</xsl:text>
            <xsl:value-of select="$bits"/>
            <xsl:text> &lt;&lt; </xsl:text>
            <xsl:value-of select="$bit-no"/>
            <xsl:text>);</xsl:text>
          </xsl:when>
          <xsl:otherwise>
            <xsl:text>((config[</xsl:text>
            <xsl:value-of select="$byte-no"/>
            <xsl:text> + 2] |= </xsl:text>
            <xsl:text>(0b</xsl:text>
            <xsl:value-of select="$bits"/>
            <xsl:text> &lt;&lt; </xsl:text>
            <xsl:value-of select="$bit-no"/>
            <xsl:text>)); ((config[</xsl:text>
            <xsl:value-of select="$byte-no"/>
            <xsl:text> + 2] &amp;= ~</xsl:text>
            <xsl:text>(0b</xsl:text>
            <xsl:value-of select="$bits"/>
            <xsl:text> &lt;&lt; </xsl:text>
            <xsl:value-of select="$bit-no"/>
            <xsl:text>));</xsl:text>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:when>
      <xsl:otherwise>
        <xsl:variable name="divider">
          <xsl:choose>
            <xsl:when test="contains($dividers, ' ')">
              <xsl:value-of select="substring-before($dividers, ' ')"/>
            </xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$dividers"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:variable>
        <xsl:variable name="rest-dividers" select="substring-after($dividers, ' ')"/>
        <xsl:apply-templates select="." mode="create-option-setters">
          <xsl:with-param name="dividers"></xsl:with-param>
          <xsl:with-param name="bits" select="substring($bits, 1, $divider)"/>
          <xsl:with-param name="bit-no" select="$bit-no"/>
          <xsl:with-param name="byte-no" select="$byte-no"/>
        </xsl:apply-templates>
        <xsl:variable name="rest-bits" select="substring($bits, $divider + 1)"/>
        <xsl:if test="$rest-bits != ''">
          <xsl:text> </xsl:text>
          <xsl:apply-templates select="." mode="create-option-setters">
            <xsl:with-param name="dividers" select="$rest-dividers"/>
            <xsl:with-param name="byte-no" select="$byte-no + 1"/>
            <xsl:with-param name="bit-no" select="0"/>
            <xsl:with-param name="bits" select="$rest-bits"/>
          </xsl:apply-templates>
        </xsl:if>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  <xsl:template name="create-byte-mask">
    <xsl:param name="bits"/>
    <xsl:param name="position" select="0"/>
    <xsl:if test="$position &lt; 8">
      <xsl:call-template name="create-byte-mask">
        <xsl:with-param name="bits" select="$bits"/>
        <xsl:with-param name="position" select="$position + 1"/>
      </xsl:call-template>
      <xsl:choose>
        <xsl:when test="$position &lt; $bits">1</xsl:when>
        <xsl:otherwise>0</xsl:otherwise>
      </xsl:choose>
    </xsl:if>
  </xsl:template>
</xsl:stylesheet>
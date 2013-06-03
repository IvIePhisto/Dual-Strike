<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:c="urn:strike-devices:configuration"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance">
  
  <xsl:template match="c:configuration">
    <xsl:copy>
      <xsl:variable name="bit-width">
        <xsl:apply-templates select="descendant::c:*[local-name() = 'boolean' or local-name() = 'choice'][1]" mode="calculate-bit-width"/>
      </xsl:variable>
      <xsl:attribute name="byte-width">
        <xsl:value-of select="ceiling($bit-width div 7)"/>
      </xsl:attribute>
      <xsl:apply-templates select="@*"/>
      <xsl:apply-templates select="node()"/>
    </xsl:copy>
  </xsl:template>
  
  <xsl:template match="c:choice" mode="calculate-bit-width">
    <xsl:variable name="bit-width">
      <xsl:call-template name="calculate-bit-width">
        <xsl:with-param name="value" select="count(c:option) - 1"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:choose>
      <xsl:when test="following::c:*[local-name() = 'boolean' or local-name() = 'choice'][1]">
        <xsl:variable name="following-bit-width">
          <xsl:apply-templates select="following::c:*[local-name() = 'boolean' or local-name() = 'choice'][1]" mode="calculate-bit-width"/>
        </xsl:variable>
        <xsl:value-of select="$following-bit-width + $bit-width"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$bit-width"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  <xsl:template match="c:boolean" mode="calculate-bit-width">
    <xsl:choose>
      <xsl:when test="following::c:*[local-name() = 'boolean' or local-name() = 'choice'][1]">
        <xsl:variable name="following-bit-width">
          <xsl:apply-templates select="following::c:*[local-name() = 'boolean' or local-name() = 'choice'][1]" mode="calculate-bit-width"/>
        </xsl:variable>
        <xsl:value-of select="$following-bit-width + 1"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="1"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  <xsl:template match="c:configuration/@xsi:schemaLocation">
    <xsl:variable name="postfix">/configuration-def.xsd</xsl:variable>
    <xsl:if test="substring(., string-length(.) - string-length($postfix) + 1) = $postfix">
      <xsl:attribute name="schemaLocation" namespace="http://www.w3.org/2001/XMLSchema-instance">
        <xsl:value-of select="substring(., 1, string-length(.) - string-length($postfix))"/>
        <xsl:text>/annotated-configuration-def.xsd</xsl:text>
      </xsl:attribute>
    </xsl:if>
  </xsl:template>

  <xsl:template match="c:choice">
    <xsl:variable name="currentChoiceID" select="generate-id(current())"/>

    <xsl:for-each select="c:option">
      <xsl:variable name="firstID" select="current()/@id"/>
      <xsl:variable name="firstIsRequiring" select="count(current()/c:requires) > 0"/>  
          
      <xsl:for-each select="/c:configuration/c:page/c:choice[c:option/c:requires[string() = $firstID]]">
        <xsl:variable name="firstChoiceID" select="generate-id(current())"/>
    
        <xsl:if test="$firstIsRequiring">
          <xsl:message terminate="yes">
            <xsl:text>The choice A contains the option with ID "</xsl:text>
            <xsl:value-of select="$firstID"/>
            <xsl:text>" which is being required by the option with ID "</xsl:text>
            <xsl:value-of select="c:option/c:requires[string() = $firstID][1]"/>
            <xsl:text>"of another choice B, but an option of A requires a setting.</xsl:text>
          </xsl:message>
        </xsl:if>

        <xsl:for-each select="/c:configuration/c:page/c:choice[generate-id() = $currentChoiceID]/c:option">
          <xsl:variable name="secondID" select="current()/@id"/>
          
          <xsl:for-each select="/c:configuration/c:page/c:choice[generate-id() != $firstChoiceID][c:option/c:requires[string() = $secondID]]">
              <xsl:message terminate="yes">
                <xsl:text>In one choice A the option with ID "</xsl:text>
                <xsl:value-of select="$firstID"/>
                <xsl:text>" is required by another choice B and the option in A with ID "</xsl:text>
                <xsl:value-of select="$secondID"/>
                <xsl:text>" is required by a third choice C.</xsl:text>
              </xsl:message>
          </xsl:for-each>
        </xsl:for-each>
      </xsl:for-each>
    </xsl:for-each>
    
    <xsl:copy>
      <xsl:variable name="bit-width">
        <xsl:call-template name="calculate-bit-width">
          <xsl:with-param name="value" select="count(c:option) - 1"/>
        </xsl:call-template>
      </xsl:variable>
      <xsl:attribute name="bit-width">
        <xsl:value-of select="$bit-width"/>
      </xsl:attribute>
      <xsl:variable name="previous-bits">
        <xsl:choose>
          <xsl:when test="preceding::c:*[local-name() = 'choice' or local-name() = 'boolean'][1]">
            <xsl:apply-templates select="preceding::c:*[local-name() = 'choice' or local-name() = 'boolean'][1]" mode="calculate-previous-bits"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="0"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>
      <xsl:variable name="byte-dividers">
        <xsl:call-template name="calculate-byte-dividers">
          <xsl:with-param name="current-byte-index" select="$previous-bits mod 7"/>
          <xsl:with-param name="bit-width" select="$bit-width"/>
        </xsl:call-template>
      </xsl:variable>
      <xsl:if test="$byte-dividers != ''">
        <xsl:attribute name="byte-dividers">
          <xsl:value-of select="$byte-dividers"/>
        </xsl:attribute>
      </xsl:if>
      <xsl:attribute name="byte-no">
        <xsl:value-of select="floor($previous-bits div 7)"/>
      </xsl:attribute>
      <xsl:attribute name="bit-no">
        <xsl:value-of select="$previous-bits mod 7"/>
      </xsl:attribute>
      <xsl:apply-templates select="@*"/>
      <xsl:apply-templates select="node()">
        <xsl:with-param name="bit-width" select="$bit-width"/>
      </xsl:apply-templates>
    </xsl:copy>
  </xsl:template>
  
  <xsl:template match="c:boolean">
    <xsl:copy>
      <xsl:variable name="previous-bits">
        <xsl:choose>
          <xsl:when test="preceding::c:*[local-name() = 'choice' or local-name() = 'boolean'][1]">
            <xsl:apply-templates select="preceding::c:*[local-name() = 'choice' or local-name() = 'boolean'][1]" mode="calculate-previous-bits"/>
          </xsl:when>
          <xsl:otherwise>
            <xsl:value-of select="0"/>
          </xsl:otherwise>
        </xsl:choose>
      </xsl:variable>      
      <xsl:attribute name="byte-no">
        <xsl:value-of select="floor($previous-bits div 7)"/>
      </xsl:attribute>
      <xsl:attribute name="bit-no">
        <xsl:value-of select="$previous-bits mod 7"/>
      </xsl:attribute>
      <xsl:apply-templates select="@*"/>
      <xsl:apply-templates select="node()"/>
    </xsl:copy>
  </xsl:template>
  
  <xsl:template match="c:choice" mode="calculate-previous-bits">
    <xsl:variable name="bit-width">
        <xsl:call-template name="calculate-bit-width">
          <xsl:with-param name="value" select="count(c:option) - 1"/>
        </xsl:call-template>
    </xsl:variable>
    
    <xsl:choose>
      <xsl:when test="preceding::c:*[local-name() = 'choice' or local-name() = 'boolean'][1]">
        <xsl:variable name="rest-sum">
          <xsl:apply-templates select="preceding::c:*[local-name() = 'choice' or local-name() = 'boolean'][1]" mode="calculate-previous-bits"/>
        </xsl:variable>
        <xsl:value-of select="$rest-sum + $bit-width"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="$bit-width"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>
  
  <xsl:template match="c:boolean" mode="calculate-previous-bits">
    <xsl:choose>
      <xsl:when test="preceding::c:*[local-name() = 'choice' or local-name() = 'boolean'][1]">
        <xsl:variable name="rest-sum">
            <xsl:apply-templates select="preceding::c:*[local-name() = 'choice' or local-name() = 'boolean'][1]" mode="calculate-previous-bits"/>
        </xsl:variable>
        <xsl:value-of select="$rest-sum + 1"/>
      </xsl:when>
      <xsl:otherwise>
        <xsl:value-of select="1"/>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template name="calculate-bit-width">
    <xsl:param name="value"/>
    <xsl:variable name="binary-value">
      <xsl:call-template name="calculate-binary-number">
        <xsl:with-param name="value" select="$value"/>
      </xsl:call-template>
    </xsl:variable>
    <xsl:value-of select="string-length($binary-value)"/>
  </xsl:template>
  
  <xsl:template name="calculate-binary-number">
    <xsl:param name="value"/>
    <xsl:param name="remaining-bits" select="-1"/>
    <xsl:variable name="rest" select="$value mod 2"/>
    <xsl:variable name="new-value" select="($value - $rest) div 2"/>
    <xsl:if test="($remaining-bits != -1 and $remaining-bits > 1) or ($new-value > 0)">
      <xsl:call-template name="calculate-binary-number">
        <xsl:with-param name="value" select="$new-value"/>
        <xsl:with-param name="remaining-bits">
          <xsl:choose>
            <xsl:when test="$remaining-bits = -1">-1</xsl:when>
            <xsl:otherwise>
              <xsl:value-of select="$remaining-bits - 1"/>
            </xsl:otherwise>
          </xsl:choose>
        </xsl:with-param>
      </xsl:call-template>
    </xsl:if>
    <xsl:value-of select="$rest"/>
  </xsl:template>
  
  <xsl:template name="calculate-byte-dividers">
    <xsl:param name="current-byte-index" select="0"/>
    <xsl:param name="bit-width"/>
    <xsl:if test="($current-byte-index + $bit-width) > 7">
      <xsl:variable name="rest" select="($current-byte-index + $bit-width) - 7"/>
      <xsl:value-of select="$bit-width - $rest"/>
      <xsl:if test="$rest > 7">
        <xsl:text> </xsl:text>
        <xsl:call-template name="calculate-byte-dividers">
          <xsl:with-param name="bit-width" select="$rest"/>
        </xsl:call-template>
      </xsl:if>
    </xsl:if>
  </xsl:template>

  <xsl:template match="c:option">
    <xsl:param name="bit-width"/>
    <xsl:copy>
      <xsl:attribute name="bit-pattern">
        <xsl:call-template name="calculate-binary-number">
          <xsl:with-param name="value" select="count(preceding-sibling::c:option)"/>
          <xsl:with-param name="remaining-bits" select="$bit-width"/>
        </xsl:call-template>
      </xsl:attribute>
      <xsl:apply-templates select="@*"/>
      <xsl:apply-templates select="node()"/>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="
    c:configuration/@byte-width |
    c:choice/@bit-width |
    c:choice/@bit-dividers |
    c:option/@bit-pattern |
    @byte-no |
    @bit-no" priority="1"/>

  <xsl:template match="@* | node()">
    <xsl:copy>
      <xsl:apply-templates select="@*"/>
      <xsl:apply-templates select="node()"/>
    </xsl:copy>
  </xsl:template>
</xsl:stylesheet>
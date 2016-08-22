<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:msxsl="urn:schemas-microsoft-com:xslt"
                extension-element-prefixes="msxsl"
                xmlns:frmwrk="Corel Framework Data"
                exclude-result-prefixes="frmwrk">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>

  <frmwrk:uiconfig>
    <frmwrk:compositeNode xPath="/uiConfig/commandBars"/>
  </frmwrk:uiconfig>

  <!-- Helper "insert" mode for applying XSLT templates -->
  <xsl:template match="node()|@*" mode="insert">
    <xsl:param name='nodes'/>
    <xsl:variable name='container' select='.'/>
    <xsl:copy>
      <xsl:apply-templates select='@*'/>
      <xsl:apply-templates select='msxsl:node-set($nodes)/beginning/*'/>
      <xsl:for-each select="./*">
        <xsl:variable name='itemGuid' select='@guid'/>
        <xsl:variable name='itemGuidRef' select='@guidRef'/>
        <xsl:apply-templates select='msxsl:node-set($nodes)/before[@guid=$itemGuid or @guidRef=$itemGuidRef]/*'/>
        <xsl:apply-templates select="."/>
        <xsl:apply-templates select='msxsl:node-set($nodes)/after[@guid=$itemGuid or @guidRef=$itemGuidRef]/*'/>
      </xsl:for-each>
      <xsl:apply-templates select='msxsl:node-set($nodes)/end/*'/>
    </xsl:copy>
  </xsl:template>

  <!-- By default, copy all the nodes -->
  <xsl:template match="node()|@*">
    <xsl:copy>
      <xsl:apply-templates select="node()|@*"/>
    </xsl:copy>
  </xsl:template>

  <!-- Property Bar -->
  <xsl:template match="uiConfig/commandBars/commandBarData[@guid='74e03d83-404c-49f5-824a-fe0fd02ab29a']/toolbar">
    <xsl:apply-templates select="." mode="insert">
      <xsl:with-param name="nodes">
        <end>
          <!-- Ruler Tool property bar mode -->
          <modeData guid="07da4f59-2cfd-4d8a-49bc-6d121ebcd47b">
            <item guidRef="01e04e8a-b8b2-88a2-4d1a-14f4184c980d"/>
          </modeData>
        </end>
      </xsl:with-param>
    </xsl:apply-templates>
  </xsl:template>

  <!-- Toolbox -->
  <xsl:template match="uiConfig/commandBars/commandBarData[@guid='7c905e2a-cb64-4ba1-aff0-c306f392680c']/toolbar">
    <xsl:apply-templates select="." mode="insert">
      <xsl:with-param name="nodes">
        <!-- Add Ruler Tool after Shape tool fly-out (which has GUID of afc9eb73-69ac-4511-be97-5c6c54588bd7) -->
        <after guidRef="afc9eb73-69ac-4511-be97-5c6c54588bd7">
          <item guidRef="a5bdc561-e254-a088-4c94-ed1eb6f8e320"/>
        </after>
      </xsl:with-param>
    </xsl:apply-templates>
  </xsl:template>

</xsl:stylesheet>
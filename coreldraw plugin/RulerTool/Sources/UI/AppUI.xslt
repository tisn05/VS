<?xml version="1.0"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:msxsl="urn:schemas-microsoft-com:xslt"
                extension-element-prefixes="msxsl"
                xmlns:frmwrk="Corel Framework Data"
                exclude-result-prefixes="frmwrk">
  <xsl:output method="xml" encoding="UTF-8" indent="yes"/>
  <frmwrk:uiconfig>
    <frmwrk:applicationInfo userConfiguration="true" />
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
  
  <!-- UI item definitions -->
  <xsl:template match="uiConfig/items">
    <xsl:apply-templates select="." mode="insert">
      <xsl:with-param name="nodes">
        <!-- Append the following items to the end of uiConfig/items node -->
        <end>
          <!-- Ruler Tool item in the Toolbox-->
          <itemData guid="a5bdc561-e254-a088-4c94-ed1eb6f8e320"
                    type="groupedRadioButton"
                    currentActiveOfGroup="*Bind(DataSource=WAppDataSource;Path=ActiveTool;BindType=TwoWay)"
                    enable="*Bind(DataSource=WAppDataSource;Path=ToolboxEnable)"
                    identifier="a5bdc561-e254-a088-4c94-ed1eb6f8e320"
                    customizationMode="28295234-2c3b-4977-80c6-c2dbbd14fe0a"/>
          <!-- Ruler Tool distance control in property bar -->
          <itemData guid="01e04e8a-b8b2-88a2-4d1a-14f4184c980d"
                    icon="guid://496ea244-5a15-4d15-bb7c-4cb1ee27db96"
                    type="spinner"
                    showSpinner="false"
                    showAmbiguous="*Not(*Bind(DataSource=RulerToolDS;Path=IsMeasuring))"
                    value="*Bind(DataSource=RulerToolDS;Path=Distance)"
                    leftLabelText="*ResS('1e2247a4-3b70-b5a3-4fb3-109d467ed64e')"
                    enable="false"
                    numDecimalPlaces="*Bind(DataSource=WPrefsApp;Path=DrawingPrecision)"
                    increment="0.1 in; 1 mm; 1pt; 1px"
                    rangeMin="0"
                    rangeMax="*Bind(DataSource=WPickDataSource;Path=PositionXMax)"
                    displayUnit="*Bind(DataSource=DocumentPrefsDS;Path=XRulerUnit)"
                    showUnit="true"
                    customizationMode="4fbdcedf-c475-4719-91f2-beb193169730"
                    scale="*Bind(DataSource=DocumentPrefsDS;Path=WorldScale)"
                    />
        </end>
      </xsl:with-param>
    </xsl:apply-templates>
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
        <!-- Or try the Shape tool itself without fly-out (this is the case for Draw Essentials) -->
        <after guidRef="ae9f4ba1-2406-4dbc-b119-c12f9cca17f7">
          <item guidRef="a5bdc561-e254-a088-4c94-ed1eb6f8e320"/>
        </after>
      </xsl:with-param>
    </xsl:apply-templates>
  </xsl:template>


</xsl:stylesheet>
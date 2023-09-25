<!-- Example XSLT stylesheet (CUnit-Run.xsl) -->

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html"/>
<xsl:template match="/">
---
# CUnit Test Report
## Test Suites

| Suite Name | Tests Run | Tests Passed | Tests Failed |
| ---------- | :-------: | :----------: | :----------: |
<xsl:for-each select="CUNIT_TEST_RUN_REPORT/CUNIT_RESULT_LISTING/CUNIT_RUN_SUITE/CUNIT_RUN_SUITE_SUCCESS">|<xsl:value-of select="SUITE_NAME"/>|<xsl:value-of select="count(CUNIT_RUN_TEST_RECORD)" />|<xsl:value-of select="count(CUNIT_RUN_TEST_RECORD/CUNIT_RUN_TEST_SUCCESS)" />|<xsl:value-of select="count(CUNIT_RUN_TEST_RECORD/CUNIT_RUN_TEST_FAILURE)" />|
</xsl:for-each>
 
<xsl:for-each select="CUNIT_TEST_RUN_REPORT/CUNIT_RESULT_LISTING/CUNIT_RUN_SUITE/CUNIT_RUN_SUITE_SUCCESS">
## <xsl:value-of select="SUITE_NAME"/>
| Test case | Result |
| --- | :---: |<xsl:for-each select="CUNIT_RUN_TEST_RECORD/CUNIT_RUN_TEST_FAILURE">
|<xsl:value-of select="TEST_NAME"/>|<span style="color:red">Failed</span>|</xsl:for-each>
<xsl:for-each select="CUNIT_RUN_TEST_RECORD/CUNIT_RUN_TEST_SUCCESS">
|<xsl:value-of select="TEST_NAME"/>|<span style="color:green">Passed</span>|</xsl:for-each>
</xsl:for-each>


---
</xsl:template>
</xsl:stylesheet>

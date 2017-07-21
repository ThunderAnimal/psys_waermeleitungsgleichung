<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>core.c</name>
    <path>W:/psys_waermeleitungsgleichung/openmp_manually/</path>
    <filename>core_8c</filename>
    <includes id="core_8h" name="core.h" local="yes" imported="no">core.h</includes>
    <member kind="function">
      <type>unsigned int</type>
      <name>reziseRasterForCenter</name>
      <anchorfile>core_8c.html</anchorfile>
      <anchor>a09155d2f0adbd5d9cd83323003881653</anchor>
      <arglist>(unsigned int rasterSize)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setUpRaster</name>
      <anchorfile>core_8c.html</anchorfile>
      <anchor>a6a29cb93c9e414b86a73708f9f17af91</anchor>
      <arglist>(float startTemperature, float cornerTemperature, unsigned int rasterSize, unsigned int startPointX, unsigned int startPointY, float **raster)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setNewRaster</name>
      <anchorfile>core_8c.html</anchorfile>
      <anchor>a22efb7c3dbfd51a7a4e34bb6802eca7e</anchor>
      <arglist>(unsigned int rasterSize, float **heatMap, float **newHeatMap)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>valideWaermeLeitfaehigkeit</name>
      <anchorfile>core_8c.html</anchorfile>
      <anchor>a2adf358be509ff29023a2e299fcdd4fc</anchor>
      <arglist>(float waermeleitfaehigkeit)</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>calcNextHeatMap</name>
      <anchorfile>core_8c.html</anchorfile>
      <anchor>acba599dd5367baf1209fed4a80efc477</anchor>
      <arglist>(unsigned int startpointX, unsigned int startPointY, unsigned int rasterSize, float waermeleitfaehigkeit, float **heatMap, float **newHeatMap)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>outputStartParameter</name>
      <anchorfile>core_8c.html</anchorfile>
      <anchor>a3ef6a173b6aa8bdbfe53cc1fbf930ad9</anchor>
      <arglist>(float startTemperature, float cornerTemperatur, float diffEndTemperatur, unsigned int rasterSize, unsigned int startPointX, unsigned int startPointY, float waermeleitfaehigkeit)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>outputSteps</name>
      <anchorfile>core_8c.html</anchorfile>
      <anchor>a29e7ee179005149394781ec7cef3865b</anchor>
      <arglist>(int steps)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>outputHeatMap</name>
      <anchorfile>core_8c.html</anchorfile>
      <anchor>af9419b5417bde849a4c5e43f867c68a5</anchor>
      <arglist>(int rasterSize, float **raster)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>outputHeadMapGui</name>
      <anchorfile>core_8c.html</anchorfile>
      <anchor>a4d580495406a81ed13673daf296c2824</anchor>
      <arglist>(int rasterSize, float **raster)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>exec_head_conduction</name>
      <anchorfile>core_8c.html</anchorfile>
      <anchor>a2025986aca271c141b48f40ae0604818</anchor>
      <arglist>(float startTemperature, float cornerTemperatur, float diffEndTemperatur, unsigned int rasterSize, unsigned int startPointX, unsigned int startPointY, float waermeleitfaehigkeit, int modus)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>head_conduction_simple</name>
      <anchorfile>core_8c.html</anchorfile>
      <anchor>a92995eb6d209b7206529d657e0cc5b85</anchor>
      <arglist>(float diffEndTemperatur, unsigned int rasterSize, float waermeleitfaehigkeit, int modus)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>core.h</name>
    <path>W:/psys_waermeleitungsgleichung/openmp_manually/</path>
    <filename>core_8h</filename>
    <member kind="function">
      <type>int</type>
      <name>exec_head_conduction</name>
      <anchorfile>core_8h.html</anchorfile>
      <anchor>a2025986aca271c141b48f40ae0604818</anchor>
      <arglist>(float startTemperature, float cornerTemperatur, float diffEndTemperatur, unsigned int rasterSize, unsigned int startPointX, unsigned int startPointY, float waermeleitfaehigkeit, int modus)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>head_conduction_simple</name>
      <anchorfile>core_8h.html</anchorfile>
      <anchor>a92995eb6d209b7206529d657e0cc5b85</anchor>
      <arglist>(float diffEndTemperatur, unsigned int rasterSize, float waermeleitfaehigkeit, int modus)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>core_test.c</name>
    <path>W:/psys_waermeleitungsgleichung/openmp_manually/</path>
    <filename>core__test_8c</filename>
    <includes id="core_8c" name="core.c" local="yes" imported="no">core.c</includes>
    <member kind="function">
      <type>void</type>
      <name>test__eingabe</name>
      <anchorfile>core__test_8c.html</anchorfile>
      <anchor>afce86f5d0aca1489297f69b235b7f04b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test__setUpRaster</name>
      <anchorfile>core__test_8c.html</anchorfile>
      <anchor>ade91f199935634dcdbc78bb86d3431d4</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test__centerHeatMapRaster</name>
      <anchorfile>core__test_8c.html</anchorfile>
      <anchor>ae479f78183b08e515b7ddb774d33a416</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test__calcNextHeatMap</name>
      <anchorfile>core__test_8c.html</anchorfile>
      <anchor>a95839be8acb9c25be015b4fea6c69b50</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>test__setNewHeatMap</name>
      <anchorfile>core__test_8c.html</anchorfile>
      <anchor>af75a98f98ab2daddf471662972910ac2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>main</name>
      <anchorfile>core__test_8c.html</anchorfile>
      <anchor>a3c04138a5bfe5d72780bb7e82a18e627</anchor>
      <arglist>(int argc, char **argv)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>heat_conduction.c</name>
    <path>W:/psys_waermeleitungsgleichung/openmp_manually/</path>
    <filename>heat__conduction_8c</filename>
    <includes id="core_8h" name="core.h" local="yes" imported="no">core.h</includes>
    <member kind="function">
      <type>int</type>
      <name>main</name>
      <anchorfile>heat__conduction_8c.html</anchorfile>
      <anchor>a3c04138a5bfe5d72780bb7e82a18e627</anchor>
      <arglist>(int argc, char **argv)</arglist>
    </member>
  </compound>
</tagfile>

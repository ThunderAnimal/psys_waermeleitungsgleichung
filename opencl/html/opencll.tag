<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>core.c</name>
    <path>W:/psys_waermeleitungsgleichung/opencl/</path>
    <filename>core_8c</filename>
    <includes id="core_8h" name="core.h" local="yes" imported="no">core.h</includes>
    <member kind="function">
      <type>unsigned int</type>
      <name>getIndexFromXY</name>
      <anchorfile>core_8c.html</anchorfile>
      <anchor>aba5578e4139b43e8f5dd98ebe9bdf06a</anchor>
      <arglist>(unsigned int pointX, unsigned int pointY, unsigned int rasterSize)</arglist>
    </member>
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
      <anchor>aef6bb2741fa37b6ec1e49081d791cb65</anchor>
      <arglist>(float startTemperature, float cornerTemperature, unsigned int rasterSize, unsigned int startPointX, unsigned int startPointY, float *raster)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setNewRaster</name>
      <anchorfile>core_8c.html</anchorfile>
      <anchor>ad744ed8b09cce91be8e785c3479d538a</anchor>
      <arglist>(float *heatMap, float *newHeatMap, size_t worksteps, size_t memworksize, cl_command_queue cq, cl_mem cl_bufferSrc, cl_mem cl_bufferDest, cl_kernel k_copy)</arglist>
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
      <anchor>a6f2d3742ea903e95b215851b74638913</anchor>
      <arglist>(float *heatMap, float *newHeatMap, float *diffTemperature, int calcSteps, size_t *worksteps, size_t memworksize, cl_command_queue cq, cl_mem cl_bufferHeatMap1, cl_mem cl_bufferHeatMap2, cl_mem cl_bufferDiffTemperature, cl_kernel k_calc)</arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>max_diffTemperature</name>
      <anchorfile>core_8c.html</anchorfile>
      <anchor>af1f4b59470179d8cbbc8d8b96061cdb5</anchor>
      <arglist>(unsigned int arraySize, float *diffTemperature)</arglist>
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
      <anchor>a26677f65d1ed41500953ea06db8780de</anchor>
      <arglist>(int rasterSize, float *raster)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>outputHeadMapGui</name>
      <anchorfile>core_8c.html</anchorfile>
      <anchor>a84e1f249a91f445df401ebc1a1bd9604</anchor>
      <arglist>(int arraySize, float *raster)</arglist>
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
    <path>W:/psys_waermeleitungsgleichung/opencl/</path>
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
    <name>heat_conduction.c</name>
    <path>W:/psys_waermeleitungsgleichung/opencl/</path>
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

<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="9.6.2">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="2" name="Route2" color="1" fill="3" visible="no" active="no"/>
<layer number="3" name="Route3" color="4" fill="3" visible="no" active="no"/>
<layer number="4" name="Route4" color="1" fill="4" visible="no" active="no"/>
<layer number="5" name="Route5" color="4" fill="4" visible="no" active="no"/>
<layer number="6" name="Route6" color="1" fill="8" visible="no" active="no"/>
<layer number="7" name="Route7" color="4" fill="8" visible="no" active="no"/>
<layer number="8" name="Route8" color="1" fill="2" visible="no" active="no"/>
<layer number="9" name="Route9" color="4" fill="2" visible="no" active="no"/>
<layer number="10" name="Route10" color="1" fill="7" visible="no" active="no"/>
<layer number="11" name="Route11" color="4" fill="7" visible="no" active="no"/>
<layer number="12" name="Route12" color="1" fill="5" visible="no" active="no"/>
<layer number="13" name="Route13" color="4" fill="5" visible="no" active="no"/>
<layer number="14" name="Route14" color="1" fill="6" visible="no" active="no"/>
<layer number="15" name="Route15" color="4" fill="6" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="88" name="SimResults" color="9" fill="1" visible="yes" active="yes"/>
<layer number="89" name="SimProbes" color="9" fill="1" visible="yes" active="yes"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
<layer number="99" name="SpiceOrder" color="7" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="frames" urn="urn:adsk.eagle:library:229">
<description>&lt;b&gt;Frames for Sheet and Layout&lt;/b&gt;</description>
<packages>
</packages>
<symbols>
<symbol name="A4L-LOC" urn="urn:adsk.eagle:symbol:13874/1" library_version="1">
<wire x1="256.54" y1="3.81" x2="256.54" y2="8.89" width="0.1016" layer="94"/>
<wire x1="256.54" y1="8.89" x2="256.54" y2="13.97" width="0.1016" layer="94"/>
<wire x1="256.54" y1="13.97" x2="256.54" y2="19.05" width="0.1016" layer="94"/>
<wire x1="256.54" y1="19.05" x2="256.54" y2="24.13" width="0.1016" layer="94"/>
<wire x1="161.29" y1="3.81" x2="161.29" y2="24.13" width="0.1016" layer="94"/>
<wire x1="161.29" y1="24.13" x2="215.265" y2="24.13" width="0.1016" layer="94"/>
<wire x1="215.265" y1="24.13" x2="256.54" y2="24.13" width="0.1016" layer="94"/>
<wire x1="246.38" y1="3.81" x2="246.38" y2="8.89" width="0.1016" layer="94"/>
<wire x1="246.38" y1="8.89" x2="256.54" y2="8.89" width="0.1016" layer="94"/>
<wire x1="246.38" y1="8.89" x2="215.265" y2="8.89" width="0.1016" layer="94"/>
<wire x1="215.265" y1="8.89" x2="215.265" y2="3.81" width="0.1016" layer="94"/>
<wire x1="215.265" y1="8.89" x2="215.265" y2="13.97" width="0.1016" layer="94"/>
<wire x1="215.265" y1="13.97" x2="256.54" y2="13.97" width="0.1016" layer="94"/>
<wire x1="215.265" y1="13.97" x2="215.265" y2="19.05" width="0.1016" layer="94"/>
<wire x1="215.265" y1="19.05" x2="256.54" y2="19.05" width="0.1016" layer="94"/>
<wire x1="215.265" y1="19.05" x2="215.265" y2="24.13" width="0.1016" layer="94"/>
<text x="217.17" y="15.24" size="2.54" layer="94">&gt;DRAWING_NAME</text>
<text x="217.17" y="10.16" size="2.286" layer="94">&gt;LAST_DATE_TIME</text>
<text x="230.505" y="5.08" size="2.54" layer="94">&gt;SHEET</text>
<text x="216.916" y="4.953" size="2.54" layer="94">Sheet:</text>
<frame x1="0" y1="0" x2="260.35" y2="179.07" columns="6" rows="4" layer="94"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="A4L-LOC" urn="urn:adsk.eagle:component:13926/1" prefix="FRAME" uservalue="yes" library_version="1">
<description>&lt;b&gt;FRAME&lt;/b&gt;&lt;p&gt;
DIN A4, landscape with location and doc. field</description>
<gates>
<gate name="G$1" symbol="A4L-LOC" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="HUM-A-900-PRO-CAS">
<packages>
<package name="XCVR_HUM-A-900-PRO-CAS">
<wire x1="-8.382" y1="12.827" x2="8.382" y2="12.827" width="0.127" layer="51"/>
<wire x1="8.382" y1="12.827" x2="8.382" y2="-12.827" width="0.127" layer="51"/>
<wire x1="8.382" y1="-12.827" x2="-8.382" y2="-12.827" width="0.127" layer="51"/>
<wire x1="-8.382" y1="-12.827" x2="-8.382" y2="12.827" width="0.127" layer="51"/>
<wire x1="-8.382" y1="12.827" x2="8.382" y2="12.827" width="0.127" layer="21"/>
<wire x1="8.382" y1="12.827" x2="8.382" y2="2.55" width="0.127" layer="21"/>
<wire x1="8.382" y1="-11.55" x2="8.382" y2="-12.827" width="0.127" layer="21"/>
<wire x1="8.382" y1="-12.827" x2="7.1" y2="-12.827" width="0.127" layer="21"/>
<wire x1="-7" y1="-12.827" x2="-8.382" y2="-12.827" width="0.127" layer="21"/>
<wire x1="-8.382" y1="-12.827" x2="-8.382" y2="-11.55" width="0.127" layer="21"/>
<wire x1="-8.382" y1="2.55" x2="-8.382" y2="5.2" width="0.127" layer="21"/>
<wire x1="-8.382" y1="9.75" x2="-8.382" y2="12.827" width="0.127" layer="21"/>
<text x="-8.5" y="13.5" size="1.27" layer="25">&gt;NAME</text>
<text x="-8.5" y="-14.5" size="1.27" layer="27" align="top-left">&gt;VALUE</text>
<wire x1="-9.0384" y1="13.077" x2="9.14" y2="13.077" width="0.05" layer="39"/>
<wire x1="9.14" y1="13.077" x2="9.14" y2="-13.585" width="0.05" layer="39"/>
<wire x1="9.14" y1="-13.585" x2="-9.0384" y2="-13.585" width="0.05" layer="39"/>
<wire x1="-9.0384" y1="-13.585" x2="-9.0384" y2="13.077" width="0.05" layer="39"/>
<circle x="-7" y="-13.9" radius="0.1" width="0.2" layer="21"/>
<circle x="-7" y="-13.9" radius="0.1" width="0.2" layer="51"/>
<smd name="22" x="8.001" y="1.8542" dx="1.778" dy="0.7112" layer="1"/>
<smd name="21" x="8.001" y="0.5842" dx="1.778" dy="0.7112" layer="1"/>
<smd name="20" x="8.001" y="-0.6858" dx="1.778" dy="0.7112" layer="1"/>
<smd name="19" x="8.001" y="-1.9558" dx="1.778" dy="0.7112" layer="1"/>
<smd name="18" x="8.001" y="-3.2258" dx="1.778" dy="0.7112" layer="1"/>
<smd name="17" x="8.001" y="-4.4958" dx="1.778" dy="0.7112" layer="1"/>
<smd name="16" x="8.001" y="-5.7658" dx="1.778" dy="0.7112" layer="1"/>
<smd name="15" x="8.001" y="-7.0358" dx="1.778" dy="0.7112" layer="1"/>
<smd name="14" x="8.001" y="-8.3058" dx="1.778" dy="0.7112" layer="1"/>
<smd name="13" x="8.001" y="-9.5758" dx="1.778" dy="0.7112" layer="1"/>
<smd name="12" x="8.001" y="-10.8458" dx="1.778" dy="0.7112" layer="1"/>
<smd name="10" x="5.1308" y="-12.446" dx="1.778" dy="0.7112" layer="1" rot="R270"/>
<smd name="9" x="3.8608" y="-12.446" dx="1.778" dy="0.7112" layer="1" rot="R270"/>
<smd name="8" x="2.5908" y="-12.446" dx="1.778" dy="0.7112" layer="1" rot="R270"/>
<smd name="7" x="1.3208" y="-12.446" dx="1.778" dy="0.7112" layer="1" rot="R270"/>
<smd name="6" x="0.0508" y="-12.446" dx="1.778" dy="0.7112" layer="1" rot="R270"/>
<smd name="5" x="-1.2192" y="-12.446" dx="1.778" dy="0.7112" layer="1" rot="R270"/>
<smd name="4" x="-2.4892" y="-12.446" dx="1.778" dy="0.7112" layer="1" rot="R270"/>
<smd name="3" x="-3.7592" y="-12.446" dx="1.778" dy="0.7112" layer="1" rot="R270"/>
<smd name="2" x="-5.0292" y="-12.446" dx="1.778" dy="0.7112" layer="1" rot="R270"/>
<smd name="1" x="-6.2992" y="-12.446" dx="1.778" dy="0.7112" layer="1" rot="R270"/>
<smd name="43" x="-7.8994" y="-9.5758" dx="1.778" dy="0.7112" layer="1" rot="R180"/>
<smd name="42" x="-7.8994" y="-8.3058" dx="1.778" dy="0.7112" layer="1" rot="R180"/>
<smd name="41" x="-7.8994" y="-7.0358" dx="1.778" dy="0.7112" layer="1" rot="R180"/>
<smd name="40" x="-7.8994" y="-5.7658" dx="1.778" dy="0.7112" layer="1" rot="R180"/>
<smd name="39" x="-7.8994" y="-4.4958" dx="1.778" dy="0.7112" layer="1" rot="R180"/>
<smd name="38" x="-7.8994" y="-3.2258" dx="1.778" dy="0.7112" layer="1" rot="R180"/>
<smd name="37" x="-7.8994" y="-1.9558" dx="1.778" dy="0.7112" layer="1" rot="R180"/>
<smd name="36" x="-7.8994" y="-0.6858" dx="1.778" dy="0.7112" layer="1" rot="R180"/>
<smd name="35" x="-7.8994" y="0.5842" dx="1.778" dy="0.7112" layer="1" rot="R180"/>
<smd name="34" x="-7.8994" y="1.8542" dx="1.778" dy="0.7112" layer="1" rot="R180"/>
<smd name="44" x="-7.8994" y="-10.8458" dx="1.778" dy="0.7112" layer="1" rot="R180"/>
<smd name="11" x="6.4008" y="-12.446" dx="1.778" dy="0.7112" layer="1" rot="R270"/>
<smd name="33" x="-7.8994" y="6.1722" dx="1.778" dy="1.2954" layer="1" rot="R180"/>
<smd name="32" x="-7.8994" y="8.7122" dx="1.778" dy="1.2954" layer="1" rot="R180"/>
</package>
</packages>
<symbols>
<symbol name="HUM-A-900-PRO-CAS">
<wire x1="17.78" y1="17.78" x2="17.78" y2="-20.32" width="0.254" layer="94"/>
<wire x1="17.78" y1="-20.32" x2="-17.78" y2="-20.32" width="0.254" layer="94"/>
<wire x1="-17.78" y1="-20.32" x2="-17.78" y2="17.78" width="0.254" layer="94"/>
<wire x1="-17.78" y1="17.78" x2="17.78" y2="17.78" width="0.254" layer="94"/>
<text x="-17.78" y="19.05" size="1.778" layer="95">&gt;NAME</text>
<text x="-17.78" y="-22.86" size="1.778" layer="96">&gt;VALUE</text>
<pin name="GND" x="22.86" y="-17.78" length="middle" direction="pwr" rot="R180"/>
<pin name="NC" x="22.86" y="-15.24" length="middle" direction="nc" rot="R180"/>
<pin name="!CMD" x="-22.86" y="5.08" length="middle" direction="in"/>
<pin name="!RESET" x="-22.86" y="7.62" length="middle" direction="in"/>
<pin name="!POWER_DOWN" x="-22.86" y="10.16" length="middle" direction="in"/>
<pin name="VCC" x="22.86" y="15.24" length="middle" direction="pwr" rot="R180"/>
<pin name="PB" x="-22.86" y="0" length="middle" direction="in"/>
<pin name="MODE_IND" x="22.86" y="-10.16" length="middle" direction="out" rot="R180"/>
<pin name="ANTENNA" x="22.86" y="10.16" length="middle" rot="R180"/>
<pin name="!CTS" x="22.86" y="-5.08" length="middle" direction="out" rot="R180"/>
<pin name="CMD_DATA_IN" x="-22.86" y="-5.08" length="middle" direction="in"/>
<pin name="BE" x="22.86" y="5.08" length="middle" direction="out" rot="R180"/>
<pin name="EX" x="22.86" y="2.54" length="middle" direction="out" rot="R180"/>
<pin name="!CRESP" x="22.86" y="-2.54" length="middle" direction="out" rot="R180"/>
<pin name="CMD_DATA_OUT" x="-22.86" y="-7.62" length="middle" direction="out"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="HUM-A-900-PRO-CAS" prefix="U">
<description>General ISM &lt; 1GHz Proprietary Transceiver Module 902MHz ~ 928MHz Antenna Not Included, Castellation Surface Mount    &lt;a href="https://pricing.snapeda.com/parts/HUM-A-900-PRO-CAS/Linx/view-part?ref=eda"&gt;Check prices&lt;/a&gt;</description>
<gates>
<gate name="G$1" symbol="HUM-A-900-PRO-CAS" x="0" y="0"/>
</gates>
<devices>
<device name="" package="XCVR_HUM-A-900-PRO-CAS">
<connects>
<connect gate="G$1" pin="!CMD" pad="35"/>
<connect gate="G$1" pin="!CRESP" pad="13"/>
<connect gate="G$1" pin="!CTS" pad="34"/>
<connect gate="G$1" pin="!POWER_DOWN" pad="43"/>
<connect gate="G$1" pin="!RESET" pad="40"/>
<connect gate="G$1" pin="ANTENNA" pad="33"/>
<connect gate="G$1" pin="BE" pad="4"/>
<connect gate="G$1" pin="CMD_DATA_IN" pad="16"/>
<connect gate="G$1" pin="CMD_DATA_OUT" pad="15"/>
<connect gate="G$1" pin="EX" pad="12"/>
<connect gate="G$1" pin="GND" pad="9 32"/>
<connect gate="G$1" pin="MODE_IND" pad="36"/>
<connect gate="G$1" pin="NC" pad="1 2 3 5 6 7 8 10 11 17 18 19 20 21 37 38 39 41 42 44"/>
<connect gate="G$1" pin="PB" pad="14"/>
<connect gate="G$1" pin="VCC" pad="22"/>
</connects>
<technologies>
<technology name="">
<attribute name="AVAILABILITY" value="Warning"/>
<attribute name="DESCRIPTION" value=" General ISM &lt; 1GHz Proprietary Transceiver Module 902MHz ~ 928MHz Antenna Not Included, Castellation Surface Mount "/>
<attribute name="MF" value="Linx"/>
<attribute name="MP" value="HUM-A-900-PRO-CAS"/>
<attribute name="PACKAGE" value="Module Linx"/>
<attribute name="PRICE" value="None"/>
<attribute name="PURCHASE-URL" value="https://pricing.snapeda.com/search/part/HUM-A-900-PRO-CAS/?ref=eda"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="ANT-916-SP">
<packages>
<package name="XDCR_ANT-916-SP">
<wire x1="-13.97" y1="13.7" x2="13.97" y2="13.7" width="0.127" layer="51"/>
<wire x1="13.97" y1="13.7" x2="13.97" y2="0" width="0.127" layer="51"/>
<wire x1="13.97" y1="0" x2="-13.97" y2="0" width="0.127" layer="51"/>
<wire x1="-13.97" y1="0" x2="-13.97" y2="13.7" width="0.127" layer="51"/>
<wire x1="-13.97" y1="1.59" x2="-13.97" y2="13.7" width="0.127" layer="21"/>
<wire x1="-13.97" y1="13.7" x2="13.97" y2="13.7" width="0.127" layer="21"/>
<wire x1="13.97" y1="13.7" x2="13.97" y2="1.59" width="0.127" layer="21"/>
<polygon width="0.01" layer="41">
<vertex x="-13.97" y="0"/>
<vertex x="-13.97" y="13.7"/>
<vertex x="13.97" y="13.7"/>
<vertex x="13.97" y="0"/>
<vertex x="13.72" y="0"/>
<vertex x="13.72" y="1.275"/>
<vertex x="11.68" y="1.275"/>
<vertex x="11.68" y="0"/>
<vertex x="8.64" y="0"/>
<vertex x="8.64" y="1.275"/>
<vertex x="6.6" y="1.275"/>
<vertex x="6.6" y="0"/>
<vertex x="3.56" y="0"/>
<vertex x="3.56" y="1.275"/>
<vertex x="1.52" y="1.275"/>
<vertex x="1.52" y="0"/>
<vertex x="-1.52" y="0"/>
<vertex x="-1.52" y="1.275"/>
<vertex x="-3.56" y="1.275"/>
<vertex x="-3.56" y="0"/>
<vertex x="-6.6" y="0"/>
<vertex x="-6.6" y="1.275"/>
<vertex x="-8.64" y="1.275"/>
<vertex x="-8.64" y="0"/>
<vertex x="-11.68" y="0"/>
<vertex x="-11.68" y="1.275"/>
<vertex x="-13.72" y="1.275"/>
<vertex x="-13.72" y="0"/>
</polygon>
<wire x1="-14.22" y1="13.95" x2="14.22" y2="13.95" width="0.05" layer="39"/>
<wire x1="14.22" y1="13.95" x2="14.22" y2="-1.52" width="0.05" layer="39"/>
<wire x1="14.22" y1="-1.52" x2="-14.22" y2="-1.52" width="0.05" layer="39"/>
<wire x1="-14.22" y1="-1.52" x2="-14.22" y2="13.95" width="0.05" layer="39"/>
<text x="-14" y="16" size="1.27" layer="25">&gt;NAME</text>
<text x="-14" y="14.5" size="1.27" layer="27">&gt;VALUE</text>
<circle x="-14.52" y="-0.4" radius="0.1" width="0.2" layer="21"/>
<circle x="-14.52" y="-0.4" radius="0.1" width="0.2" layer="51"/>
<polygon width="0.01" layer="43">
<vertex x="-13.97" y="0"/>
<vertex x="-13.97" y="13.7"/>
<vertex x="13.97" y="13.7"/>
<vertex x="13.97" y="0"/>
<vertex x="13.72" y="0"/>
<vertex x="13.72" y="1.275"/>
<vertex x="11.68" y="1.275"/>
<vertex x="11.68" y="0"/>
<vertex x="8.64" y="0"/>
<vertex x="8.64" y="1.275"/>
<vertex x="6.6" y="1.275"/>
<vertex x="6.6" y="0"/>
<vertex x="3.56" y="0"/>
<vertex x="3.56" y="1.275"/>
<vertex x="1.52" y="1.275"/>
<vertex x="1.52" y="0"/>
<vertex x="-1.52" y="0"/>
<vertex x="-1.52" y="1.275"/>
<vertex x="-3.56" y="1.275"/>
<vertex x="-3.56" y="0"/>
<vertex x="-6.6" y="0"/>
<vertex x="-6.6" y="1.275"/>
<vertex x="-8.64" y="1.275"/>
<vertex x="-8.64" y="0"/>
<vertex x="-11.68" y="0"/>
<vertex x="-11.68" y="1.275"/>
<vertex x="-13.72" y="1.275"/>
<vertex x="-13.72" y="0"/>
</polygon>
<smd name="1" x="-12.7" y="0" dx="2.03" dy="2.54" layer="1"/>
<smd name="2" x="-7.62" y="0" dx="2.03" dy="2.54" layer="1"/>
<smd name="3" x="-2.54" y="0" dx="2.03" dy="2.54" layer="1"/>
<smd name="4" x="2.54" y="0" dx="2.03" dy="2.54" layer="1"/>
<smd name="5" x="7.62" y="0" dx="2.03" dy="2.54" layer="1"/>
<smd name="6" x="12.7" y="0" dx="2.03" dy="2.54" layer="1"/>
</package>
</packages>
<symbols>
<symbol name="ANT-916-SP">
<wire x1="-1.397" y1="3.175" x2="0" y2="3.175" width="0.254" layer="94"/>
<wire x1="0" y1="3.175" x2="1.397" y2="3.175" width="0.254" layer="94"/>
<wire x1="1.397" y1="3.175" x2="0" y2="0" width="0.254" layer="94"/>
<wire x1="0" y1="0" x2="-1.397" y2="3.175" width="0.254" layer="94"/>
<wire x1="0" y1="3.175" x2="0" y2="0" width="0.254" layer="94"/>
<wire x1="0" y1="0" x2="0" y2="-2.54" width="0.254" layer="94"/>
<wire x1="0" y1="-2.54" x2="-1.27" y2="-2.54" width="0.254" layer="94"/>
<wire x1="0" y1="-2.54" x2="1.27" y2="-2.54" width="0.254" layer="94"/>
<wire x1="-0.762" y1="-3.302" x2="0.762" y2="-3.302" width="0.254" layer="94"/>
<wire x1="-0.127" y1="-4.064" x2="0.254" y2="-4.064" width="0.254" layer="94"/>
<text x="2.03443125" y="0.7629125" size="1.27151875" layer="95">&gt;NAME</text>
<text x="2.16275" y="-1.01776875" size="1.272209375" layer="96">&gt;VALUE</text>
<pin name="FEEDING" x="-2.54" y="0" visible="pad" length="short" direction="pas"/>
<pin name="GND" x="-2.54" y="-2.54" visible="pad" length="short" direction="pwr"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="ANT-916-SP" prefix="E">
<description>315MHz Alarms, ISM, Remote Control, Security Systems Chip RF Antenna 312MHz ~ 317MHz Solder Surface Mount  &lt;a href="https://pricing.snapeda.com/parts/ANT-916-SP/Linx%20Technologies/view-part?ref=eda"&gt;Check prices&lt;/a&gt;</description>
<gates>
<gate name="G$1" symbol="ANT-916-SP" x="0" y="0"/>
</gates>
<devices>
<device name="" package="XDCR_ANT-916-SP">
<connects>
<connect gate="G$1" pin="FEEDING" pad="1"/>
<connect gate="G$1" pin="GND" pad="2 3 4 5 6"/>
</connects>
<technologies>
<technology name="">
<attribute name="AVAILABILITY" value="Warning"/>
<attribute name="DESCRIPTION" value=" 915 MHz Monopole Chip Antenna "/>
<attribute name="MF" value="Linx Technologies"/>
<attribute name="MP" value="ANT-916-SP"/>
<attribute name="PACKAGE" value="ANTENNA-1 Linx Technologies"/>
<attribute name="PRICE" value="None"/>
<attribute name="PURCHASE-URL" value="https://pricing.snapeda.com/search/part/ANT-916-SP/?ref=eda"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="4-644694-4">
<packages>
<package name="TE_4-644694-4">
<wire x1="-1.118" y1="-1.168" x2="-1.118" y2="1.168" width="0.127" layer="51"/>
<wire x1="-1.118" y1="1.168" x2="34.138" y2="1.168" width="0.127" layer="51"/>
<wire x1="34.138" y1="1.168" x2="34.138" y2="-1.168" width="0.127" layer="51"/>
<wire x1="34.138" y1="-1.168" x2="-1.118" y2="-1.168" width="0.127" layer="51"/>
<wire x1="-1.118" y1="-1.168" x2="-1.118" y2="-7.772" width="0.127" layer="51"/>
<wire x1="-1.118" y1="-7.772" x2="34.138" y2="-7.772" width="0.127" layer="51"/>
<wire x1="34.138" y1="-7.772" x2="34.138" y2="-1.168" width="0.127" layer="51"/>
<wire x1="-1.118" y1="-1.168" x2="-1.118" y2="1.168" width="0.127" layer="21"/>
<wire x1="34.138" y1="1.168" x2="34.138" y2="-1.168" width="0.127" layer="21"/>
<wire x1="34.138" y1="1.168" x2="-1.118" y2="1.168" width="0.127" layer="21"/>
<wire x1="34.138" y1="-1.168" x2="-1.118" y2="-1.168" width="0.127" layer="21"/>
<wire x1="-1.368" y1="-8.022" x2="-1.368" y2="1.418" width="0.05" layer="39"/>
<wire x1="-1.368" y1="1.418" x2="34.388" y2="1.418" width="0.05" layer="39"/>
<wire x1="34.388" y1="1.418" x2="34.388" y2="-8.022" width="0.05" layer="39"/>
<wire x1="34.388" y1="-8.022" x2="-1.368" y2="-8.022" width="0.05" layer="39"/>
<text x="-2.118" y="2.418" size="1.27" layer="25">&gt;NAME</text>
<text x="-2.118" y="-10.022" size="1.27" layer="27">&gt;VALUE</text>
<circle x="-1.768" y="0" radius="0.1" width="0.2" layer="21"/>
<circle x="-1.768" y="0" radius="0.1" width="0.2" layer="51"/>
<pad name="1" x="0" y="0" drill="0.94" diameter="1.29" shape="square"/>
<pad name="2" x="2.54" y="0" drill="0.94" diameter="1.29"/>
<pad name="3" x="5.08" y="0" drill="0.94" diameter="1.29"/>
<pad name="4" x="7.62" y="0" drill="0.94" diameter="1.29"/>
<pad name="5" x="10.16" y="0" drill="0.94" diameter="1.29"/>
<pad name="6" x="12.7" y="0" drill="0.94" diameter="1.29"/>
<pad name="7" x="15.24" y="0" drill="0.94" diameter="1.29"/>
<pad name="8" x="17.78" y="0" drill="0.94" diameter="1.29"/>
<pad name="9" x="20.32" y="0" drill="0.94" diameter="1.29"/>
<pad name="10" x="22.86" y="0" drill="0.94" diameter="1.29"/>
<pad name="11" x="25.4" y="0" drill="0.94" diameter="1.29"/>
<pad name="12" x="27.94" y="0" drill="0.94" diameter="1.29"/>
<pad name="13" x="30.48" y="0" drill="0.94" diameter="1.29"/>
<pad name="14" x="33.02" y="0" drill="0.94" diameter="1.29"/>
</package>
</packages>
<symbols>
<symbol name="4-644694-4">
<wire x1="-5.08" y1="-17.78" x2="-5.08" y2="20.32" width="0.254" layer="94"/>
<wire x1="-5.08" y1="20.32" x2="5.08" y2="20.32" width="0.254" layer="94"/>
<wire x1="5.08" y1="20.32" x2="5.08" y2="-17.78" width="0.254" layer="94"/>
<wire x1="5.08" y1="-17.78" x2="-5.08" y2="-17.78" width="0.254" layer="94"/>
<text x="-5.58" y="20.32" size="1.778" layer="95">&gt;NAME</text>
<text x="-5.08" y="-20.32" size="1.778" layer="96">&gt;VALUE</text>
<pin name="1" x="-10.16" y="17.78" length="middle" direction="pas"/>
<pin name="2" x="-10.16" y="15.24" length="middle" direction="pas"/>
<pin name="3" x="-10.16" y="12.7" length="middle" direction="pas"/>
<pin name="4" x="-10.16" y="10.16" length="middle" direction="pas"/>
<pin name="5" x="-10.16" y="7.62" length="middle" direction="pas"/>
<pin name="6" x="-10.16" y="5.08" length="middle" direction="pas"/>
<pin name="7" x="-10.16" y="2.54" length="middle" direction="pas"/>
<pin name="8" x="-10.16" y="0" length="middle" direction="pas"/>
<pin name="9" x="-10.16" y="-2.54" length="middle" direction="pas"/>
<pin name="10" x="-10.16" y="-5.08" length="middle" direction="pas"/>
<pin name="11" x="-10.16" y="-7.62" length="middle" direction="pas"/>
<pin name="12" x="-10.16" y="-10.16" length="middle" direction="pas"/>
<pin name="13" x="-10.16" y="-12.7" length="middle" direction="pas"/>
<pin name="14" x="-10.16" y="-15.24" length="middle" direction="pas"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="4-644694-4" prefix="J">
<description> &lt;a href="https://pricing.snapeda.com/parts/4-644694-4/TE%20Connectivity/view-part?ref=eda"&gt;Check prices&lt;/a&gt;</description>
<gates>
<gate name="G$1" symbol="4-644694-4" x="0" y="0"/>
</gates>
<devices>
<device name="" package="TE_4-644694-4">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="10" pad="10"/>
<connect gate="G$1" pin="11" pad="11"/>
<connect gate="G$1" pin="12" pad="12"/>
<connect gate="G$1" pin="13" pad="13"/>
<connect gate="G$1" pin="14" pad="14"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
<connect gate="G$1" pin="7" pad="7"/>
<connect gate="G$1" pin="8" pad="8"/>
<connect gate="G$1" pin="9" pad="9"/>
</connects>
<technologies>
<technology name="">
<attribute name="AVAILABILITY" value="Good"/>
<attribute name="CENTERLINE_PITCH" value="2.54 mm[.1 in]"/>
<attribute name="COMMENT" value="4-644694-4"/>
<attribute name="DESCRIPTION" value=" 14P MTA100 R/A HDR RETENTIVE "/>
<attribute name="EU_ROHS_COMPLIANCE" value="Compliant"/>
<attribute name="MF" value="TE Connectivity"/>
<attribute name="MP" value="4-644694-4"/>
<attribute name="NUMBER_OF_POSITIONS" value="14"/>
<attribute name="PACKAGE" value="None"/>
<attribute name="PRICE" value="None"/>
<attribute name="PRODUCT_TYPE" value="Connector"/>
<attribute name="PURCHASE-URL" value="https://pricing.snapeda.com/search/part/4-644694-4/?ref=eda"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="TAJA106K010RNJ">
<packages>
<package name="CAPMP3216X180N">
<description>&lt;b&gt;TANTALUM CAPACITOR&lt;/b&gt;</description>
<wire x1="-2.455" y1="1.05" x2="2.455" y2="1.05" width="0.05" layer="39"/>
<wire x1="2.455" y1="-1.05" x2="-2.455" y2="-1.05" width="0.05" layer="39"/>
<wire x1="-2.455" y1="-1.05" x2="-2.455" y2="1.05" width="0.05" layer="39"/>
<wire x1="2.455" y1="1.05" x2="2.455" y2="-1.05" width="0.05" layer="39"/>
<wire x1="-1.6" y1="0.8" x2="-1.6" y2="-0.8" width="0.127" layer="51"/>
<wire x1="-1.6" y1="-0.8" x2="1.6" y2="-0.8" width="0.127" layer="51"/>
<wire x1="1.6" y1="-0.8" x2="1.6" y2="0.8" width="0.127" layer="51"/>
<wire x1="1.6" y1="0.8" x2="-1.6" y2="0.8" width="0.127" layer="51"/>
<text x="-2.5" y="1.5" size="1.016" layer="25">&gt;NAME</text>
<text x="-2.5" y="-1.5" size="1.016" layer="27" align="top-left">&gt;VALUE</text>
<wire x1="-1.6" y1="0.98" x2="1.6" y2="0.98" width="0.127" layer="21"/>
<wire x1="-1.6" y1="-0.98" x2="1.6" y2="-0.98" width="0.127" layer="21"/>
<circle x="-2.75" y="0" radius="0.1" width="0.2" layer="21"/>
<circle x="-2.75" y="0" radius="0.1" width="0.2" layer="51"/>
<smd name="1" x="-1.3" y="0" dx="1.81" dy="1.32" layer="1" roundness="25"/>
<smd name="2" x="1.3" y="0" dx="1.81" dy="1.32" layer="1" roundness="25"/>
</package>
</packages>
<symbols>
<symbol name="TAJA106K010RNJ">
<text x="-5.08" y="3.81" size="1.27296875" layer="95">&gt;NAME</text>
<text x="-5.08" y="-5.08" size="1.27213125" layer="96">&gt;VALUE</text>
<wire x1="-0.254" y1="2.54" x2="-0.254" y2="-2.54" width="0.254" layer="94" curve="70"/>
<wire x1="-1.016" y1="0" x2="2.54" y2="0" width="0.254" layer="94"/>
<wire x1="-5.08" y1="0" x2="-2.032" y2="0" width="0.254" layer="94"/>
<wire x1="-2.032" y1="2.54" x2="-2.032" y2="-2.54" width="0.254" layer="94"/>
<rectangle x1="-3.713" y1="-1.532" x2="-2.824" y2="-1.405" layer="94"/>
<rectangle x1="-3.332" y1="-1.913" x2="-3.205" y2="-1.024" layer="94"/>
<pin name="1" x="-5.08" y="0" visible="pad" length="point" direction="pas"/>
<pin name="2" x="2.54" y="0" visible="pad" length="point" direction="pas" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="TAJA106K010RNJ" prefix="C">
<description>1µF Molded Tantalum Capacitors 16V 1206 (3216 Metric) 11Ohm @ 100kHz  &lt;a href="https://pricing.snapeda.com/parts/TAJA106K010RNJ/AVX%20Interconnect%20/%20Elco/view-part?ref=eda"&gt;Check prices&lt;/a&gt;</description>
<gates>
<gate name="G$1" symbol="TAJA106K010RNJ" x="0" y="0"/>
</gates>
<devices>
<device name="" package="CAPMP3216X180N">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name="">
<attribute name="AVAILABILITY" value="Warning"/>
<attribute name="DESCRIPTION" value=" 10µF Molded Tantalum Capacitors 10V 1206 (3216 Metric) 3Ohm "/>
<attribute name="MF" value="AVX Interconnect / Elco"/>
<attribute name="MP" value="TAJA106K010RNJ"/>
<attribute name="PACKAGE" value="1206 AVX"/>
<attribute name="PRICE" value="None"/>
<attribute name="PURCHASE-URL" value="https://pricing.snapeda.com/search/part/TAJA106K010RNJ/?ref=eda"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="ngspice-simulation" urn="urn:adsk.eagle:library:527439">
<description>SPICE compatible library parts</description>
<packages>
</packages>
<symbols>
<symbol name="0" urn="urn:adsk.eagle:symbol:527455/1" library_version="18">
<description>Simulation ground symbol (spice node 0)</description>
<wire x1="-2.54" y1="0" x2="2.54" y2="0" width="0.1524" layer="94"/>
<wire x1="2.54" y1="0" x2="0" y2="-2.54" width="0.1524" layer="94"/>
<wire x1="0" y1="-2.54" x2="-2.54" y2="0" width="0.1524" layer="94"/>
<pin name="0" x="0" y="0" visible="off" length="point" direction="sup"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="GND" urn="urn:adsk.eagle:component:527478/1" prefix="X_" library_version="18">
<description>Simulation ground symbol (spice node 0)</description>
<gates>
<gate name="G$1" symbol="0" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name="">
<attribute name="SPICEGROUND" value=""/>
<attribute name="_EXTERNAL_" value=""/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="FRAME1" library="frames" library_urn="urn:adsk.eagle:library:229" deviceset="A4L-LOC" device=""/>
<part name="U1" library="HUM-A-900-PRO-CAS" deviceset="HUM-A-900-PRO-CAS" device=""/>
<part name="E1" library="ANT-916-SP" deviceset="ANT-916-SP" device=""/>
<part name="J1" library="4-644694-4" deviceset="4-644694-4" device=""/>
<part name="C1" library="TAJA106K010RNJ" deviceset="TAJA106K010RNJ" device="" value="10uF"/>
<part name="X_1" library="ngspice-simulation" library_urn="urn:adsk.eagle:library:527439" deviceset="GND" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="FRAME1" gate="G$1" x="0" y="0" smashed="yes">
<attribute name="DRAWING_NAME" x="217.17" y="15.24" size="2.54" layer="94"/>
<attribute name="LAST_DATE_TIME" x="217.17" y="10.16" size="2.286" layer="94"/>
<attribute name="SHEET" x="230.505" y="5.08" size="2.54" layer="94"/>
</instance>
<instance part="U1" gate="G$1" x="86.36" y="106.68" smashed="yes">
<attribute name="NAME" x="68.58" y="125.73" size="1.778" layer="95"/>
<attribute name="VALUE" x="68.58" y="83.82" size="1.778" layer="96"/>
</instance>
<instance part="E1" gate="G$1" x="185.42" y="142.24" smashed="yes">
<attribute name="NAME" x="187.45443125" y="143.0029125" size="1.27151875" layer="95"/>
<attribute name="VALUE" x="187.58275" y="141.22223125" size="1.272209375" layer="96"/>
</instance>
<instance part="J1" gate="G$1" x="190.5" y="104.14" smashed="yes">
<attribute name="NAME" x="184.92" y="124.46" size="1.778" layer="95"/>
<attribute name="VALUE" x="185.42" y="83.82" size="1.778" layer="96"/>
</instance>
<instance part="C1" gate="G$1" x="129.54" y="116.84" smashed="yes" rot="R270">
<attribute name="NAME" x="133.35" y="119.38" size="1.27296875" layer="95" rot="R270"/>
<attribute name="VALUE" x="124.46" y="119.38" size="1.27213125" layer="96" rot="R270"/>
</instance>
<instance part="X_1" gate="G$1" x="129.54" y="111.76" smashed="yes"/>
</instances>
<busses>
</busses>
<nets>
<net name="ANT" class="0">
<segment>
<pinref part="U1" gate="G$1" pin="ANTENNA"/>
<wire x1="114.3" y1="116.84" x2="109.22" y2="116.84" width="0.1524" layer="91"/>
<label x="114.3" y="116.84" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="E1" gate="G$1" pin="FEEDING"/>
<wire x1="182.88" y1="142.24" x2="177.8" y2="142.24" width="0.1524" layer="91"/>
<label x="177.8" y="142.24" size="1.778" layer="95" rot="R180"/>
</segment>
</net>
<net name="GND" class="0">
<segment>
<wire x1="182.88" y1="139.7" x2="177.8" y2="139.7" width="0.1524" layer="91"/>
<pinref part="E1" gate="G$1" pin="GND"/>
<label x="177.8" y="139.7" size="1.778" layer="95" rot="R180"/>
</segment>
<segment>
<pinref part="U1" gate="G$1" pin="GND"/>
<wire x1="109.22" y1="88.9" x2="114.3" y2="88.9" width="0.1524" layer="91"/>
<label x="114.3" y="88.9" size="1.778" layer="95"/>
</segment>
<segment>
<pinref part="C1" gate="G$1" pin="2"/>
<wire x1="129.54" y1="114.3" x2="129.54" y2="111.76" width="0.1524" layer="91"/>
<pinref part="X_1" gate="G$1" pin="0"/>
</segment>
<segment>
<pinref part="J1" gate="G$1" pin="14"/>
<wire x1="180.34" y1="88.9" x2="175.26" y2="88.9" width="0.1524" layer="91"/>
<label x="175.26" y="88.9" size="1.778" layer="95" rot="R180"/>
</segment>
</net>
<net name="EX" class="0">
<segment>
<wire x1="175.26" y1="119.38" x2="180.34" y2="119.38" width="0.1524" layer="91"/>
<pinref part="J1" gate="G$1" pin="2"/>
<label x="175.26" y="119.38" size="1.778" layer="95" rot="R180"/>
</segment>
<segment>
<pinref part="U1" gate="G$1" pin="EX"/>
<wire x1="109.22" y1="109.22" x2="114.3" y2="109.22" width="0.1524" layer="91"/>
<label x="114.3" y="109.22" size="1.778" layer="95"/>
</segment>
</net>
<net name="CRESP" class="0">
<segment>
<wire x1="175.26" y1="116.84" x2="180.34" y2="116.84" width="0.1524" layer="91"/>
<pinref part="J1" gate="G$1" pin="3"/>
<label x="175.26" y="116.84" size="1.778" layer="95" rot="R180"/>
</segment>
<segment>
<pinref part="U1" gate="G$1" pin="!CRESP"/>
<wire x1="109.22" y1="104.14" x2="114.3" y2="104.14" width="0.1524" layer="91"/>
<label x="114.3" y="104.14" size="1.778" layer="95"/>
</segment>
</net>
<net name="PB" class="0">
<segment>
<wire x1="175.26" y1="114.3" x2="180.34" y2="114.3" width="0.1524" layer="91"/>
<pinref part="J1" gate="G$1" pin="4"/>
<label x="175.26" y="114.3" size="1.778" layer="95" rot="R180"/>
</segment>
<segment>
<pinref part="U1" gate="G$1" pin="PB"/>
<wire x1="63.5" y1="106.68" x2="58.42" y2="106.68" width="0.1524" layer="91"/>
<label x="58.42" y="106.68" size="1.778" layer="95" rot="R180"/>
</segment>
</net>
<net name="CMD_DATA_OUT" class="0">
<segment>
<wire x1="175.26" y1="111.76" x2="180.34" y2="111.76" width="0.1524" layer="91"/>
<pinref part="J1" gate="G$1" pin="5"/>
<label x="175.26" y="111.76" size="1.778" layer="95" rot="R180"/>
</segment>
<segment>
<pinref part="U1" gate="G$1" pin="CMD_DATA_OUT"/>
<wire x1="63.5" y1="99.06" x2="58.42" y2="99.06" width="0.1524" layer="91"/>
<label x="58.42" y="99.06" size="1.778" layer="95" rot="R180"/>
</segment>
</net>
<net name="CMD_DATA_IN" class="0">
<segment>
<wire x1="175.26" y1="109.22" x2="180.34" y2="109.22" width="0.1524" layer="91"/>
<pinref part="J1" gate="G$1" pin="6"/>
<label x="175.26" y="109.22" size="1.778" layer="95" rot="R180"/>
</segment>
<segment>
<pinref part="U1" gate="G$1" pin="CMD_DATA_IN"/>
<wire x1="63.5" y1="101.6" x2="58.42" y2="101.6" width="0.1524" layer="91"/>
<label x="58.42" y="101.6" size="1.778" layer="95" rot="R180"/>
</segment>
</net>
<net name="BE" class="0">
<segment>
<pinref part="U1" gate="G$1" pin="BE"/>
<wire x1="109.22" y1="111.76" x2="114.3" y2="111.76" width="0.1524" layer="91"/>
<label x="114.3" y="111.76" size="1.778" layer="95"/>
</segment>
<segment>
<wire x1="175.26" y1="121.92" x2="180.34" y2="121.92" width="0.1524" layer="91"/>
<pinref part="J1" gate="G$1" pin="1"/>
<label x="175.26" y="121.92" size="1.778" layer="95" rot="R180"/>
</segment>
</net>
<net name="PWR_DOWN" class="0">
<segment>
<wire x1="175.26" y1="104.14" x2="180.34" y2="104.14" width="0.1524" layer="91"/>
<pinref part="J1" gate="G$1" pin="8"/>
<label x="175.26" y="104.14" size="1.778" layer="95" rot="R180"/>
</segment>
<segment>
<wire x1="63.5" y1="116.84" x2="58.42" y2="116.84" width="0.1524" layer="91"/>
<pinref part="U1" gate="G$1" pin="!POWER_DOWN"/>
<label x="58.42" y="116.84" size="1.778" layer="95" rot="R180"/>
</segment>
</net>
<net name="RESET" class="0">
<segment>
<wire x1="175.26" y1="101.6" x2="180.34" y2="101.6" width="0.1524" layer="91"/>
<pinref part="J1" gate="G$1" pin="9"/>
<label x="175.26" y="101.6" size="1.778" layer="95" rot="R180"/>
</segment>
<segment>
<pinref part="U1" gate="G$1" pin="!RESET"/>
<wire x1="63.5" y1="114.3" x2="58.42" y2="114.3" width="0.1524" layer="91"/>
<label x="58.42" y="114.3" size="1.778" layer="95" rot="R180"/>
</segment>
</net>
<net name="MODE_IND" class="0">
<segment>
<wire x1="175.26" y1="99.06" x2="180.34" y2="99.06" width="0.1524" layer="91"/>
<pinref part="J1" gate="G$1" pin="10"/>
<label x="175.26" y="99.06" size="1.778" layer="95" rot="R180"/>
</segment>
<segment>
<pinref part="U1" gate="G$1" pin="MODE_IND"/>
<wire x1="109.22" y1="96.52" x2="114.3" y2="96.52" width="0.1524" layer="91"/>
<label x="114.3" y="96.52" size="1.778" layer="95"/>
</segment>
</net>
<net name="CMD" class="0">
<segment>
<wire x1="175.26" y1="96.52" x2="180.34" y2="96.52" width="0.1524" layer="91"/>
<pinref part="J1" gate="G$1" pin="11"/>
<label x="175.26" y="96.52" size="1.778" layer="95" rot="R180"/>
</segment>
<segment>
<pinref part="U1" gate="G$1" pin="!CMD"/>
<wire x1="63.5" y1="111.76" x2="58.42" y2="111.76" width="0.1524" layer="91"/>
<label x="58.42" y="111.76" size="1.778" layer="95" rot="R180"/>
</segment>
</net>
<net name="CTS" class="0">
<segment>
<wire x1="175.26" y1="93.98" x2="180.34" y2="93.98" width="0.1524" layer="91"/>
<pinref part="J1" gate="G$1" pin="12"/>
<label x="175.26" y="93.98" size="1.778" layer="95" rot="R180"/>
</segment>
<segment>
<pinref part="U1" gate="G$1" pin="!CTS"/>
<wire x1="109.22" y1="101.6" x2="114.3" y2="101.6" width="0.1524" layer="91"/>
<label x="114.3" y="101.6" size="1.778" layer="95"/>
</segment>
</net>
<net name="VCC" class="0">
<segment>
<pinref part="U1" gate="G$1" pin="VCC"/>
<pinref part="C1" gate="G$1" pin="1"/>
<wire x1="109.22" y1="121.92" x2="129.54" y2="121.92" width="0.1524" layer="91"/>
<wire x1="129.54" y1="121.92" x2="139.7" y2="121.92" width="0.1524" layer="91"/>
<junction x="129.54" y="121.92"/>
<label x="139.7" y="121.92" size="1.778" layer="95"/>
</segment>
<segment>
<wire x1="175.26" y1="91.44" x2="180.34" y2="91.44" width="0.1524" layer="91"/>
<pinref part="J1" gate="G$1" pin="13"/>
<label x="175.26" y="91.44" size="1.778" layer="95" rot="R180"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
<compatibility>
<note version="8.2" severity="warning">
Since Version 8.2, EAGLE supports online libraries. The ids
of those online libraries will not be understood (or retained)
with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports URNs for individual library
assets (packages, symbols, and devices). The URNs of those assets
will not be understood (or retained) with this version.
</note>
</compatibility>
</eagle>

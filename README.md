<h1 align="center"><ins>CO2-Sensor with ESP8266</ins></h1>
<p float="left" align="middle">
    <img src="https://github.com/itsDkiller/CO2-Sensor/blob/master/media/product.jpg" width="400" title="Final product">
    <img src="https://github.com/itsDkiller/CO2-Sensor/blob/master/media/build.jpg" width="400" height="287" title="Final product">
</p>
<hr />
<h3 align="left">Content of this repository:</h3>
<ul>
    <li><b><a href="#credits">Credits</a></b></li>
    <li><b>General information</b></li>
    <li><b>Used parts & components</b></li>
    <li><b>Circuit diagram</b></li>
    <li><b>License</b></li>
</ul>
<hr />
<h3>Credits</h2>
<p>First things first, I didn't create this project on my own. It was planned and built in a
German workshop for young technicians by our instructor (who 3D-printed the housing and wrote the
basic software) are a handfull of electronic and mechatronic trainees, including me.
The jobs that I did only were soldering the PCBs and overwriting the software, so please don't
praise only me, if you even want to.
 
If you want to know more about this workshop, <a href="https://gbf-ev.de/beruf/verbundausbildung/">visit their website (German)</a>.</p>
<hr />
<h3>General information</h2>
<p>As you hopefully already noticed, the microcontroller for this project is a <a href="https://en.wikipedia.org/wiki/ESP8266">ESP8266</a> running the operating system <a href="https://en.wikipedia.org/wiki/NodeMCU">NodeMCU</a>. As it is programable via the Arduino IDE, we'd choose it for the CO2-sensor project. If you want to use the software for your own sensor and choose a different µC, don't forget to update the software.</p>
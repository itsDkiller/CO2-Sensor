<h1 align="center"><ins>CO2-Sensor with ESP8266</ins></h1>
<p float="left" align="middle">
    <img src="https://github.com/itsDkiller/CO2-Sensor/blob/master/media/product.jpg" width="400" title="Final product">
    <img src="https://github.com/itsDkiller/CO2-Sensor/blob/master/media/build.jpg" width="400" height="287" title="Final product">
</p>
<hr style="border:2px solid gray"> </hr>
<h3 align="left">Content of this repository:</h3>
<ul>
    <li><b><a href="#credits">Credits</a></b></li>
    <li><b><a href="#general-information">General information</a></b></li>
    <li><b><a href="#used-parts--components">Used parts and components</a></b></li>
    <li><b><a href="#circuit-diagram">Circuit diagram</a></b></li>
    <li><b>License</b></li>
</ul>
<hr />
<h3>Credits</h2>
<p>First things first, I didn't create this project on my own. It was planned and built in a
German workshop for young technicians by our instructor (who 3D-printed the housing and wrote the
basic software) and a handfull of electronic and mechatronic trainees, including me.
The jobs that I did only were soldering the PCBs and overwriting the software, so please don't
praise only me, if you even want to.
 
If you want to know more about this workshop, <a href="https://gbf-ev.de/beruf/verbundausbildung/">visit their website (German)</a>.</p>
<hr />
<h3>General information</h2>
<p>As you hopefully already noticed, the microcontroller for this project is a <a href="https://en.wikipedia.org/wiki/ESP8266">ESP8266</a> running the operating system <a href="https://en.wikipedia.org/wiki/NodeMCU">NodeMCU</a>. As it is programable via the Arduino IDE, we'd choose it for the CO2-sensor project. If you want to use the software for your own sensor and choose a different µC, don't forget to update the software.</p>
<p><b>Powering. </b>The circuit is getting powered through a tiny hole on the side of the housing, were the Micro-USB port of the µC is located.
The power for all external compenents is served through that port, so you won't need a second power supply.
Please note that the ESP8266 only serves 3,3 Volts on its power pins. As the MHZ19B sensor has an operating volate of 5 Volts, it gets powered through the V<sub>in</sub> pin of the µC.
<br /><b>Note that you can't power this circuit via your PCs USB port.</b> Because of.. high power consumption thingy..</p>
<hr />
<h3>Used parts & components</h3>
<table>
    <tr>
        <th>Component</th>
        <th>Name</th>
        <th>Purpose</th>
        <th>Documentation</th>
    </tr>
    <tr>
        <td>Microcontroller</td>
        <td>ESP8266 NodeMCU</td>
        <td>Controlls the circuit via software</td>
        <td><a href="https://arduino-esp8266.readthedocs.io/en/latest/">Link</a></td>
    </tr>
    <tr>
        <td>Display</td>
        <td>LCD I2C 128x64p NoName</td>
        <td>Displays messages while operating</td>
        <td><a href="https://randomnerdtutorials.com/guide-for-oled-display-with-arduino/">Link</a></td>
    </tr>
    <tr>
        <td>Siren</td>
        <td>3,3-5V Piezo Buzzer</td>
        <td>Honks when you have to ventilate</td>
        <td><a href="https://www.puiaudio.com/media/SpecSheet/AI-3035-TWT-3V-R.pdf">Link</a></td>
    </tr>
    <tr>
        <td>CO2-Sensor</td>
        <td>MHZ-19B</td>
        <td>Measures the current CO2 on a spot</td>
        <td><a href="https://www.winsen-sensor.com/d/files/infrared-gas-sensor/mh-z19b-co2-ver1_0.pdf">Link</a></td>
    </tr>
</table>
<p><b>LEDs.</b> When using LEDs in circuits, always be sure to calculate the current limiting resistor properly.<br />
R<sub>RESISTOR</sub> = ( U<sub>OUT</sub> - U<sub>LED</sub> ) / I<sub>MAX</sub><br />U<sub>OUT</sub> = 3,3V<br />U<sub>LED</sub> ≈ 2V (Insert the nominal voltage of your LED)<br />I<sub>MAX</sub> = 10mA</p>
<br />
<p><b>Other components. </b><a href="https://en.wikipedia.org/wiki/Perfboard">Perforated boards, </a><a href="https://en.wikipedia.org/wiki/Jump_wire">dupon cables, </a><a href="https://en.wikipedia.org/wiki/Hot-melt_adhesive">hot glue </a>and some basic electronic tools.</p>
<hr />
<h3>Circuit diagram</h3>
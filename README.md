<h1> PunchIN - A Smart Attendance Recording System </h1>
<br>

<p align="center">
    <img src="images/punchIN_banner_flat.png" alt="Logo" width="912" height="282">
</p>

<h2> About the Project </h2>
Tired of taking attendance by hand? This project will surely help you out! PunchIN is a real-time raspberry Pi project which aims to simplify the process of taking attendance by introducing a smart device. It not only saves time but also ensures accuracy and reduces the chance of manual errors. This system eliminates the need for manual signatures and allows students to mark their attendance in just a few seconds.
<br>
<br>

<h2> Join us on social media </h2>

<a href="https://twitter.com/PunchIN_38">
  <img align="left" alt="PunchIN | Twitter" width="38px" style ="margin-right:20px"  src="https://raw.githubusercontent.com/peterthehan/peterthehan/master/assets/twitter.svg" />
</a>

<a href="https://www.youtube.com/channel/UC1_89wKcrga4XP-w_oYpf4w">
  <img align="left" alt="PunchIN | YouTube" width="38px" style ="margin-right:20px" src="https://raw.githubusercontent.com/peterthehan/peterthehan/master/assets/youtube.svg" />
</a>

<br>

<h2> <br> Features </h2>

<h3> 1. Take attendance with three methods </h3>

* RFID / NFC
* Facial recognition
* Fingerprint

Maximum Flexibility!

<br>

<h3> 2. Friendly UI design </h3>

Intuitive UI design with three operation modes:

- Attendance Taking mode
- User Registration mode 
- Class Scheduling mode

<br>

<h3> 3. Email notification </h3>

- A confirmation email will be sent out automatically when a student has punched in
- A reminder email will be sent to students who registered in a class but did not show up on time

<br>

<h3> 4. Real-time attendance record </h3>

All scheduled classes and attendance records are saved into the local database. Course instructors can review the real-time attendance record in the front panel at any time.

<br>

<h2> Hardware </h2>

1. Raspberry Pi 4B 2G (Microprocessor)
2. 7" 800*480 TN Touch Monitor (Front panel)
3. PN532 RFID/NFC Reader (Sensor)
4. 90 degrees Camera (Sensor)
5. AS608 Optical Fingerprint Sensor (Sensor)

<br>

<h2> System Design </h2>

1. Circuit Diagram

<img src="images/circuit_diagram.png" alt="circuit_diagram" width="800" height="355" style="margin-left:25px">
<br>
<br>

2. Sequence Diagram

<img src="images/Sequence_diagram.png" alt="Sequence_diagram" width="800" style="margin-left:25px">
<br>

<br>
<br>

<h2> Getting Started </h2>

<h3> Software </h3>
<h4> Prerequisites </h4>

1. E-mail

<br>

2. Database
<br>

```
sudo apt-get install sqlite3 libsqlite3-dev
```

<br>

3. UI
<br>

## Required packages

Install the QT5 and Qwt development packages:

```
    sudo apt-get install qtdeclarative5-dev-tools
    sudo apt-get install libqwt-qt5-dev
    sudo apt-get install qtbase5-dev
```

<br>

4. GPIO
<br>
<h4> Installation </h4>

## Building the project

```
    mkdir build && cd build
    cmake ..
    make
```

## Running the project

```
    ./punchin
```

<br>

<h3> Hardware </h3>
<h4> Prerequisites </h4>

1. RFID/NFC-PN532

2. Fingerprint-AS608

<br>
<h4> Installation </h4>

1. RFID/NFC-PN532
<br>

2. Fingerprint-AS608
<br>
<br>

<h2> Contributors </h2>
<a href="https://github.com/lkobnas/Attendance-recording-system/graphs/contributors">
  <img width="38px" style ="margin-right:20px" src="https://contrib.rocks/image?repo=lkobnas/Attendance-recording-system" />
</a>

<br>

([lkobnas](https://github.com/lkobnas)) - San Bok Lam (2426734L) <br>
([37Sniper](https://github.com/37Sniper)) - Chenguang Wang (2826966W) <br>
([HengyuY](https://github.com/HengyuY)) - Hengyu Yang (2822511Y) <br>
([Yyyyy0512](https://github.com/Yyyyy0512)) - Jin Yang (2803454Y) <br>
<br>
<br>

<h2> Test </h2>

<br>

<h2> Project Planning </h2>

[GitHub Projects - PunchIN Project Planning](https://github.com/users/lkobnas/projects/1)
<br>
<br>

<h2> References </h2>

GPIO Pinout of Raspberry Pi 4B (2G)
<br>

<img src="images/GPIO_pinout.png" alt="raspberry_pinout" width="774" height="435">

<br>
<br>


<h2> Acknowledgements </h2>

* Logo Design and Inspiration  ([Wix](https://www.wix.com/logo/maker))
* Creating Icon ([Flaticon](https://www.flaticon.com/))
* Making contributor section ([contributor.img](https://contrib.rocks/))
* Creating graph and diagram ([Lucidchart](https://www.lucidchart.com/pages/))


<!-- MARKDOWN LINKS & IMAGES -->



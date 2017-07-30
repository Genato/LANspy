# LANspy
MFC project 

<br>

This is MFC application for searching through LAN and there is few options that you can choose like search, load or save to database.
<br><br>
In order to setup project there is few steps you need to do before you could use it.<br><br>

1. Create database and table (MS sql - i use SQLEXPRESS but there isn't difference between express and standard version)

    ~~~
    CREATE DATABASE "LANspy";

    CREATE TABLE "IPaddresses"
    (
    "IPaddress" varchar(50),
    "Hostname" varchar(100),
    "MACaddress" varchar(100),
    "SSID" varchar(50)
    );
    ~~~
    
<br>

2. Create new Systen DSN in ODBC called **NWP** and configure it to connect to database you created in first step. 

- u Control Panel-u dodajte ODBC data source (System DSN) koji pokazuje na tu bazu <br>
- koristite "Data Sources (ODBC)" tool iz Control Panel \ Administrative Tools <br>
- na 64-bitnim Windowsima koristite 32-bitnu verziju (c:\Windows\SysWOW64\odbcad32.exe) <br>
- Napravite DSN name koji odgovara projektu, nazovite ga  **NWP** <br>

<br>

_...still in development._ <br>
_...will come back with more guidance._

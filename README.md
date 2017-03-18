# LANspy
MFC project 

<br>

This is MFC application for searching through LAN and there is few options that you can choose like search, load or save to database.<br>
In order to setup project there is few steps you need to do before you could use it.<br><br>

1. Create database and table (MS sql - i use SQLEXPRESS but there isn't difference between express and standard version)

    ~~~
    CREATE DATABASE "LANspy";

    CREATE TABLE "IPaddresses"
    (
    "IPaddress" varchar(50),
    "Hostname" varchar(100),
    "MACaddress" varchar(100)
    );
    ~~~
    
<br>

2. Create new Systen DSN in ODBC called **NWP**

<br>

_...still in development._ <br>
_...will come back with more guidance._

# Webserv 42 Paris 09 / 06 / 2022 ~ 03 / 09 / 2022

Git wiki
https://github.com/zeroet/Webserv.wiki.git
 
## 📝 PDF
- [**`FILE LINK`**](https://github.com/zeroet/Webserv/en.subject.pdf)



---
## 🚩Team
---
Server Config : cjung-mo, hyungyoo.   
SocketI/O : seyun.  
Request Parsing: keulee.   
Resoponse Parsing: eyoo.   



## 🚀Content

---
## Projet
---
This is a Ecole 42 Project with C++. This project implements a simple web server based on HTTP/1.1 protocol.  
We tried to implement a web server similar to nginx as much as possible.  
It implements Get, Post, and Delete methods.  
This server provoide static web page and dynamic pages with CGI(PHP).  
The Configuration file was inspired by Nginx format and the server contorls event management with Epoll socket I/O.

---
## Building
---

You can run Linux Os because, we used Epoll library.

``$> git clone git@github.com:zeroet/Webserv.git``

``$> cd Webserv && make``

``$> ./webserv``

This will serve a local demo page on ``http://localhost:8080``

---


#!/bin/bash

mkdir -p /etc/umtprd

cp umtprd.conf /etc/umtprd/
cp usb-mtp.service /etc/systemd/system


#!/bin/bash

ssh-keygen -f '/home/nemdos/.ssh/known_hosts' -R '192.168.42.69'
ssh Panagou@192.168.42.69

#! /bin/bash
export JENKINS_HOME=/local/Jenkins/Jenkins_Home
java -XX:PermSize=256M -XX:MaxPermSize=1024M -Xmn128M -Xms512M -Xmx1024M  -jar ./jenkins.war

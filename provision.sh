#!/usr/bin/env bash

apt-get update
apt-get safe-upgrade -y
apt-get install -y gcc g++ liblapack-dev libcppunit-dev gnuplot git make
apt-get autoclean

cd /vagrant

wget -c https://workspace.imperial.ac.uk/people/Public/chemicalengineering/b.chachuat/Codes/MC++_1.0_distrib_24-07-2013.tgz
tar xzf MC++_1.0_distrib_24-07-2013.tgz

wget -c http://www.ti3.tu-harburg.de/keil/profil/Profil-2.0.8.tgz
tar xzf Profil-2.0.8.tgz
cp /vagrant/fixed_install_scripts/profil/Configure /vagrant/Profil-2.0.8/
cd Profil-2.0.8; ./Configure ; make; sudo make install

wget -c http://www2.math.uni-wuppertal.de/~xsc/software/filib++/filibsrc-3.0.2.tar.gz
tar xzf filibsrc-3.0.2.tar.gz
cd filibsrc; ./configure; make; sudo make install; cd /vagrant

cp /vagrant/fixed_install_scripts/mc++/makeoptions.mk MC++_1.0/src/
cd MC++_1.0/src; ./configure; make; sudo make install


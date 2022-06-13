import cherrypy
#from ROOT import TCanvas, TPad, TFormula, TF1, TPaveLabel, TH1F, TFile, TTree
#from ROOT import gROOT, gBenchmark, THttpServer, TFile, gROOT, AddressOf
#from array import array
#import random
import json
import string
#import threading
from datetime import datetime
from configparser import ConfigParser 
import json

ConfigFileOutput = "config.ini"

class GGM(object):

    @cherrypy.expose
    def index(self):
        return open('index.html')

    @cherrypy.expose
    def read(self):
        configur = ConfigParser() 
        configur.read(ConfigFileOutput)
        Events = configur.get('daq','Events')
        BoardAddress = configur.get('daq','BoardAddress')
        Board = configur.get('daq','Board')
        Link = configur.get('daq','Link')
        DataFolder = configur.get('daq','DataFolder')
        FileNamePrefix = configur.get('daq','FileNamePrefix')
        FileNameSuffix = configur.get('daq','FileNameSuffix')
        Terminal = configur.get('daq','Terminal')
        RootFile = configur.get('daq','RootFile')
        NumberOfRuns = configur.get('daq','NumberOfRuns')
        SpyChannel = configur.get('analysis','SpyChannel')

        f = open(DataFolder+"running.txt", "r")

        return json.dumps({'Events': Events, 
        	'BoardAddress':BoardAddress,
        	'Board':Board,
        	'Link':Link,
        	'DataFolder':DataFolder,
        	'FileNamePrefix':FileNamePrefix,
        	'FileNameSuffix':FileNameSuffix,
        	'Terminal':Terminal,
        	'RootFile':RootFile,
        	'NumberOfRuns':NumberOfRuns,
        	'SpyChannel':SpyChannel,
            'CurrentlyRead': f.readline()})
        
    @cherrypy.expose
    def generator(self,Events,BoardAddress, Board, Link,DataFolder,FileNamePrefix,FileNameSuffix,Terminal,RootFile,NumberOfRuns,SpyChannel):
        conf_file = open(ConfigFileOutput,"w") 

        sDAQheader = "[daq]"
        sEvents = "Events = "+ Events
        sBoardAddress = "BoardAddress = " + BoardAddress
        sBoard = "Board = " + Board
        sLink = "Link = " + Link
        sDataFolder = "DataFolder = " + DataFolder
        sFileNamePrefix = "FileNamePrefix = " + FileNamePrefix
        sFileNameSuffix = "FileNameSuffix = " + FileNameSuffix
        sTerminal = "Terminal = " + Terminal
        sRootFile = "RootFile = " + RootFile
        sNumberOfRuns = "NumberOfRuns = " + NumberOfRuns

        sANALYSISheader = "[analysis]"
        sSpyChannel = "SpyChannel = " + SpyChannel

        conf_file.writelines(sDAQheader+'\n') 
        conf_file.writelines(sEvents+'\n') 
        conf_file.writelines(sBoardAddress+'\n') 
        conf_file.writelines(sBoard+'\n') 
        conf_file.writelines(sLink+'\n') 
        conf_file.writelines(sDataFolder+'\n') 
        conf_file.writelines(sFileNamePrefix+'\n') 
        conf_file.writelines(sFileNameSuffix+'\n') 
        conf_file.writelines(sTerminal+'\n') 
        conf_file.writelines(sRootFile+'\n') 
        conf_file.writelines(sNumberOfRuns+'\n') 
        conf_file.writelines(sANALYSISheader+'\n') 
        conf_file.writelines(sSpyChannel+'\n') 
  
        conf_file.close()

        dateTimeObj = datetime.now()
        timestampStr = dateTimeObj.strftime("%d-%b-%Y (%H:%M:%S.%f)")

        return "Configuration properly written on " + timestampStr
cherrypy.quickstart(GGM())

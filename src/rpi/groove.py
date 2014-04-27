#!/usr/bin/env python
import time
import httplib
import StringIO
import hashlib
import uuid
import random
import string
import sys
import os
import subprocess
import gzip
import threading
import requests
import re
import fcntl
from time import sleep
from array import array
from multiprocessing import Process
if sys.version_info[1] >= 6:  import json
else: import simplejson as json
from subprocess import Popen, PIPE, check_output, check_call, CalledProcessError
try:
    from subprocess import DEVNULL # py3k
except ImportError:
    import os
    DEVNULL = open(os.devnull, 'wb')

_useragent = "Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/536.5 (KHTML, like Gecko) Chrome/19.0.1084.56 Safari/536.5"
_token = None
webappURL = "http://128.46.74.201:8000/songs/requestNext/"
URL = "grooveshark.com" #The base URL of Grooveshark
SPI_SIZE = 20
htmlclient = ('htmlshark', '20130520', 'nuggetsOfBaller', {"User-Agent":_useragent, "Content-Type":"application/json", "Accept-Encoding":"gzip"}) #Contains all the information posted with the htmlshark client
jsqueue = ['jsqueue', '20130520', 'chickenFingers']
jsqueue.append({"User-Agent":_useragent, "Referer": 'http://%s/JSQueue.swf?%s' % (URL, jsqueue[1]), "Accept-Encoding":"gzip", "Content-Type":"application/json"}) #Contains all the information specific to jsqueue

#Setting the static header (country, session and uuid)
h = {}
h["country"] = {}
h["country"]["CC1"] = 72057594037927940
h["country"]["CC2"] = 0
h["country"]["CC3"] = 0
h["country"]["CC4"] = 0
h["country"]["ID"] = 57
h["country"]["IPR"] = 0
h["privacy"] = 0
h["session"] = (''.join(random.choice(string.digits + string.letters[:6]) for x in range(32))).lower()
h["uuid"] = str.upper(str(uuid.uuid4()))

#The string that is shown when the program loads
entrystring = \
    """Welcome"""

#Generate a token from the method and the secret string (which changes once in a while)
def prepToken(method, secret):
    rnd = (''.join(random.choice(string.hexdigits) for x in range(6))).lower()
    return rnd + hashlib.sha1('%s:%s:%s:%s' % (method, _token, secret, rnd)).hexdigest()

#Fetch a queueID (right now we randomly generate it)
def getQueueID():
    return random.randint(10000000000000000000000,99999999999999999999999) #For now this will do

#Get the static token issued by sharkAttack!
def getToken():
    global h, _token
    p = {}
    p["parameters"] = {}
    p["parameters"]["secretKey"] = hashlib.md5(h["session"]).hexdigest()
    p["method"] = "getCommunicationToken"
    p["header"] = h
    p["header"]["client"] = htmlclient[0]
    p["header"]["clientRevision"] = htmlclient[1]
    conn = httplib.HTTPSConnection(URL)
    conn.request("POST", "/more.php", json.JSONEncoder().encode(p), htmlclient[3])
    _token = json.JSONDecoder().decode(gzip.GzipFile(fileobj=(StringIO.StringIO(conn.getresponse().read()))).read())["result"]

#Process a search and return the result as a list.
def getResultsFromSearch(query, what="Songs"):
    p = {}
    p["parameters"] = {}
    p["parameters"]["type"] = what
    p["parameters"]["query"] = query
    p["header"] = h
    p["header"]["client"] = htmlclient[0]
    p["header"]["clientRevision"] = htmlclient[1]
    p["header"]["token"] = prepToken("getResultsFromSearch", htmlclient[2])
    p["method"] = "getResultsFromSearch"
    conn = httplib.HTTPConnection(URL)
    conn.request("POST", "/more.php?" + p["method"], json.JSONEncoder().encode(p), htmlclient[3])
    j = json.JSONDecoder().decode(gzip.GzipFile(fileobj=(StringIO.StringIO(conn.getresponse().read()))).read())
    try:
        return j["result"]["result"]["Songs"]
    except:
        return j["result"]["result"]

#Get all songs by a certain artist
def artistGetSongsEx(id, isVerified):
    p = {}
    p["parameters"] = {}
    p["parameters"]["artistID"] = id
    p["parameters"]["isVerifiedOrPopular"] = isVerified
    p["header"] = h
    p["header"]["client"] = htmlclient[0]
    p["header"]["clientRevision"] = htmlclient[1]
    p["header"]["token"] = prepToken("artistGetSongsEx", htmlclient[2])
    p["method"] = "artistGetSongsEx"
    conn = httplib.HTTPConnection(URL)
    conn.request("POST", "/more.php?" + p["method"], json.JSONEncoder().encode(p), htmlclient[3])
    return json.JSONDecoder().decode(gzip.GzipFile(fileobj=(StringIO.StringIO(conn.getresponse().read()))).read())

#Get the streamKey used to download the songs off of the servers.
def getStreamKeyFromSongIDs(id):
    p = {}
    p["parameters"] = {}
    p["parameters"]["type"] = 8
    p["parameters"]["mobile"] = False
    p["parameters"]["prefetch"] = False
    p["parameters"]["songIDs"] = [id]
    p["parameters"]["country"] = h["country"]
    p["header"] = h
    p["header"]["client"] = jsqueue[0]
    p["header"]["clientRevision"] = jsqueue[1]
    p["header"]["token"] = prepToken("getStreamKeysFromSongIDs", jsqueue[2])
    p["method"] = "getStreamKeysFromSongIDs"
    conn = httplib.HTTPConnection(URL)
    conn.request("POST", "/more.php?" + p["method"], json.JSONEncoder().encode(p), jsqueue[3])
    return json.JSONDecoder().decode(gzip.GzipFile(fileobj=(StringIO.StringIO(conn.getresponse().read()))).read())["result"]

#Add a song to the browser queue, used to imitate a browser
def addSongsToQueue(songId, songQueueID, source = "user"):    
    queueObj = {}
    queueObj["songID"] = songId
    queueObj["source"] = source
    queueObj["songQueueSongID"] = 1
    p = {}
    p["parameters"] = {}
    p["parameters"]["songIDsArtistIDs"] = [queueObj]
    p["parameters"]["songQueueID"] = songQueueID
    p["header"] = h
    p["header"]["client"] = jsqueue[0]
    p["header"]["clientRevision"] = jsqueue[1]
    p["header"]["token"] = prepToken("addSongsToQueue", jsqueue[2])
    p["method"] = "addSongsToQueue"
    conn = httplib.HTTPConnection(URL)
    conn.request("POST", "/more.php?" + p["method"], json.JSONEncoder().encode(p), jsqueue[3])
    return json.JSONDecoder().decode(gzip.GzipFile(fileobj=(StringIO.StringIO(conn.getresponse().read()))).read())["result"]

#Remove a song from the browser queue, used to imitate a browser, in conjunction with the one above.
def removeSongsFromQueue(songQueueID, userRemoved = True):
    p = {}
    p["parameters"] = {}
    p["parameters"]["songQueueID"] = songQueueID
    p["parameters"]["userRemoved"] = True
    p["parameters"]["songQueueSongIDs"]=[1]
    p["header"] = h
    p["header"]["client"] = jsqueue[0]
    p["header"]["clientRevision"] = jsqueue[1]
    p["header"]["token"] = prepToken("removeSongsFromQueue", jsqueue[2])
    p["method"] = "removeSongsFromQueue"
    conn = httplib.HTTPConnection(URL)
    conn.request("POST", "/more.php?" + p["method"], json.JSONEncoder().encode(p), jsqueue[3])
    return json.JSONDecoder().decode(gzip.GzipFile(fileobj=(StringIO.StringIO(conn.getresponse().read()))).read())["result"]

#Mark the song as being played more then 30 seconds, used if the download of a songs takes a long time.
def markStreamKeyOver30Seconds(songID, songQueueID, streamServer, streamKey):
    p = {}
    p["parameters"] = {}
    p["parameters"]["songQueueID"] = songQueueID
    p["parameters"]["streamServerID"] = streamServer
    p["parameters"]["songID"] = songID
    p["parameters"]["streamKey"] = streamKey
    p["parameters"]["songQueueSongID"] = 1
    p["header"] = h
    p["header"]["client"] = jsqueue[0]
    p["header"]["clientRevision"] = jsqueue[1]
    p["header"]["token"] = prepToken("markStreamKeyOver30Seconds", jsqueue[2])
    p["method"] = "markStreamKeyOver30Seconds"
    conn = httplib.HTTPConnection(URL)
    conn.request("POST", "/more.php?" + p["method"], json.JSONEncoder().encode(p), jsqueue[3])
    return json.JSONDecoder().decode(gzip.GzipFile(fileobj=(StringIO.StringIO(conn.getresponse().read()))).read())["result"]

#Mark the song as downloaded, hopefully stopping us from getting banned.
def markSongDownloadedEx(streamServer, songID, streamKey):
    p = {}
    p["parameters"] = {}
    p["parameters"]["streamServerID"] = streamServer
    p["parameters"]["songID"] = songID
    p["parameters"]["streamKey"] = streamKey
    p["header"] = h
    p["header"]["client"] = jsqueue[0]
    p["header"]["clientRevision"] = jsqueue[1]
    p["header"]["token"] = prepToken("markSongDownloadedEx", jsqueue[2])
    p["method"] = "markSongDownloadedEx"
    conn = httplib.HTTPConnection(URL)
    conn.request("POST", "/more.php?" + p["method"], json.JSONEncoder().encode(p), jsqueue[3])
    return json.JSONDecoder().decode(gzip.GzipFile(fileobj=(StringIO.StringIO(conn.getresponse().read()))).read())["result"]

# TRAVIS'S ORIGINAL CODE ******************************************************************

# we'll call this function both for dl/buffering purposes
# as well as for a "next song" request
def getTopFive(pull="False"):

    # let's do a request to the webapp's 'API'
    try:
        r = requests.get(webappURL, params={'key':'team2', 'pull':pull})
    except:
        print "Error: unable to perform API request (exception occurred)"
        return []
    if r.status_code != 200:
        print "Error: communication with webapp API was unsuccessful"
        return []
    return r.text.split(',')

def downloadSong(songID):
    queueID = getQueueID()
    print "processing: ", songID

    addSongsToQueue(songID, queueID) #Add the song to the queue
    print "Retrieving stream key for song with id: ", songID
    stream = getStreamKeyFromSongIDs(songID) #Get the StreamKey for the selected song
    for k,v in stream.iteritems():
        stream=v
        if stream == []:
            print "Failed"
            exit()

        # Run wget to download the song
        cmd1 = 'wget --post-data=streamKey=%s -O "%s.mp3" "http://%s/stream.php"' % (stream["streamKey"], songID, stream["ip"]) 
        p1 = Popen(cmd1, shell=True)
        
        #Starts a timer that reports the song as being played for over 30-35 seconds. May not be needed.
        markTimer = threading.Timer(30 + random.randint(0,5), markStreamKeyOver30Seconds, [songID, str(queueID), stream["ip"], stream["streamKey"]]) 
        markTimer.start()
        try:
            p1.wait() #Wait for wget to finish, this can take a little while
        except KeyboardInterrupt: #If we are interrupted by the user
            os.remove('%s.mp3' % (songID)) #Delete the song
            print "\nDownload cancelled. File deleted."
            markTimer.cancel()

        print "Marking song as completed"
        markSongDownloadedEx(stream["ip"], songID, stream["streamKey"]) #This is the important part, hopefully this will stop grooveshark from banning us.

# runs in its own process and periodically DL's top 5
def downloader():

    # init an empty BAG so we don't re-download anything
    downloads = {}
    getToken() #Get a static token

    # put all existing mp3's in the list of stuff NOT to re-DL
    for mp3 in os.listdir("."):
        if mp3.endswith(".mp3"):
            downloads[mp3.split('.')[0]] = True

    # loop which refreshes every X seconds to re-dl
    refreshing = True
    while refreshing:
        songIdList = getTopFive()
        print songIdList
        for currId in songIdList:
            if currId in downloads: # skip, don't redownload
                continue
            downloadSong(currId)

            # mark as downloaded in the bag
            downloads[currId] = True

        print "downloader about to sleep"
        time.sleep(5)

def getMetadata(playback_process):

    # load top song in paused state and get metadata
    playback_process.stdin.write("lp " + topFive[0] + ".mp3\n")
    consumeStdOut(playback_process, 1)
    print("lp success\n")
    
    # get title, artist, album, year
    # only take first 20 chars of each
    title, artist, album, year, genre = "", "", "", "", ""
    spaceString = "                    "
    temp = consumeStdOut(playback_process, 1)
    while (not re.match('@P 1', temp)):
    	if (re.search('ID3v2.title', temp)):
    		splitTemp = temp.split(':')[1].strip()
    		title += splitTemp[:(20,len(splitTemp))[len(splitTemp) < 20]]
    	if (re.search('ID3v2.artist', temp)):
    		splitTemp = temp.split(':')[1].strip()
    		artist += splitTemp[:(20,len(splitTemp))[len(splitTemp) < 20]]
    	if (re.search('ID3v2.album', temp)):
    		splitTemp = temp.split(':')[1].strip()
    		album += splitTemp[:(20,len(splitTemp))[len(splitTemp) < 20]]
    	if (re.search('ID3v2.year', temp)):
    		splitTemp = temp.split(':')[1].strip()
    		year += splitTemp[:(20,len(splitTemp))[len(splitTemp) < 20]]
    	if (re.search('ID3v2.genre', temp)):
    		splitTemp = temp.split(':')[1].strip()
    		genre += splitTemp[:(20,len(splitTemp))[len(splitTemp) < 20]]
    	temp = playback_process.stdout.readline()


    # append spaces to strings so all strings are padded out to 
    # width of LCD line
    title += spaceString[:(SPI_SIZE-len(title))]
    print title + "end"
    artist += spaceString[:(SPI_SIZE-len(artist))]
    print artist + "end"
    album += spaceString[:(SPI_SIZE-len(album))]
    print album + "end"
    year += spaceString[:(SPI_SIZE-len(year))]
    print year + "end"
    genre += spaceString[:(SPI_SIZE-len(genre))]
    print genre + "end"
    print "\ngot metadata"
    
    # 'silent' output from mpg123 (culls constant heartbeat messages)
    playback_process.stdin.write("silence\n")
    consumeStdOut(playback_process, 1)

    # PLAY!
    playback_process.stdin.write("p\n")
    consumeStdOut(playback_process, 2)
    return title, artist, album, year, genre

def consumeStdOut(proc, numLines):
    stdoutCount = 0
    outputString = ""
    while (stdoutCount != numLines):
        try:
            outputString += proc.stdout.readline()
        except IOError:
            pass
        else:
            stdoutCount += 1
    return outputString

# MAIN ******************************************************************
if __name__ == "__main__":

    # start downloading process
    dl = Process(target=downloader)
    dl.start()

    # start up playback process
    playback = Popen(['mpg123', '-R'], shell=False, stdin=PIPE, stdout=PIPE, stderr=None)
    fcntl.fcntl(playback.stdout.fileno(), fcntl.F_SETFL, os.O_NONBLOCK)

    # init playback flag to 0
    songPlaying = 0

    # main loop to check both SPI and playback (song may have ended naturally)
    while True:

        # check to see if spi_comm exec exists
        if not os.path.isfile("spi_comm"):
            print "Cannot call spi_comm"
        if songPlaying:

            # natural end of a song will put '@P#' on stdou
            print "playing" 
            songEnd = ''
            try:
                songEnd = playback.stdout.read(2)
            except IOError: 
                pass

            if songEnd == '@P':
                print "\nEND SONG!!!!!!!!!!!!!!!!!!!!!!!!\n"
                # get top song and DL it if need be
                topFive = getTopFive()
                downloads = {}
                for mp3 in os.listdir("."):
                    if mp3.endswith(".mp3"):
                        downloads[mp3.split('.')[0]] = True
                if topFive[0] not in downloads:
                    downloadSong(topFive[0])
                    
                # get top song metadata
                title, artist, album, year, genre = getMetadata(playback)

                sleep(0.1) # sleep a tiny bit just in case micro needs
                check_call(["./spi_comm", title, artist, album, year])
                getTopFive(True) # reset top song's vote (small race condition here)
                songPlaying = 1
			
        #SPI_COMM will return 1 when a new song is requested
        try :
            check_call(["./spi_comm"])
        except CalledProcessError as cpe:
            sleep(0.5)
            print "Return code ", cpe.returncode

            # returncode of 1 means heartbeat from micro
            if cpe.returncode == 1:
                pass

            # returncode of 2 means next song
            elif cpe.returncode == 2:
                
                # get top song and DL it if need be
                topFive = getTopFive()
                downloads = {}
                for mp3 in os.listdir("."):
                    if mp3.endswith(".mp3"):
                        downloads[mp3.split('.')[0]] = True
                if topFive[0] not in downloads:
                    downloadSong(topFive[0])
                    
                # get top song metadata
                title, artist, album, year, genre = getMetadata(playback)
                print "got out of getmeta"
                sleep(0.1) # sleep a tiny bit just in case micro needs
                check_call(["./spi_comm", title, artist, album, year])
                getTopFive(True) # reset top song's votes (race condition)
                songPlaying = 1

            # play/pause
            elif cpe.returncode == 3:
                playback.stdin.write("p\n")

                # toggle playback flag, read the stdout that results
                songPlaying ^= 1
                consumeStdOut(playback, 1)
            else:
                print "Successful Call to spi_mm"
                sleep(0.5)
        time.sleep(1)

#Natural Exit
        

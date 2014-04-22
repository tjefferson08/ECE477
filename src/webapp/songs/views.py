from django.shortcuts import render, get_object_or_404
from songs.models import Song
from django.http import HttpResponseRedirect, HttpResponse, Http404
from django.core.urlresolvers import reverse
from django.views import generic
from django.utils import timezone
import requests

# home/landing page - shows top ten and search box
class IndexView(generic.ListView):
    model = Song
    template_name = 'songs/index.html'
    context_object_name = 'latest_song_list'
    def get_queryset(self):
        return Song.objects.order_by('-votes')[:10]

# once we search for, find, and vote in a song, 
# we land on the 'view results/details' page
class ResultsView(generic.DetailView):
    model = Song
    template_name = 'songs/results.html'
    context_object_name = "song"

# vote is called both from the search results vote button and the
# +1 and -1 buttons on index page, redirects to results page for
# desired-vote-song
def vote(request,song_id):

    # we should get the song in the DB from the index page, so we
    # won't be adding dupe records
    try:
        song = Song.objects.get(pk=song_id)

    # this exception should occur when we vote from the search results
    # page, in which case we'll make a new DB record and give it one vote
    except Song.DoesNotExist:
        song = Song(id_code=song_id,
                 title=request.POST['title'],
                 artist=request.POST['artist'],
                 album=request.POST['album'],
                 votes=0)

    # are we voting up or down? assume up in case of forgotten POST data
    if ('vote_up' in request.POST):
        vote_up = request.POST['vote_up']
    else:
        vote_up = "False"
        
    if (vote_up == "True"):
        song.votes += 1;
    elif (song.votes > 0): # if voting down, ensure vote # will remain nonnegative
        song.votes -= 1;

    # else, do nothing (don't vote down)
    song.save()

    # Always return an HttpResponseRedirect after successfully dealing
    # with POST data. This prevents data from being posted twice if a
    # user hits the Back button. We redirect to the song-level results/info page
    return HttpResponseRedirect(reverse('songs:results', args=(song.id_code,)))

# we call this function by a button press on index.html
def searchSong(request):
    search_string = request.POST['title']

    # default search, if request left empty
    if not search_string:
        search_string = "Never gonna give you up"

    # api concatenation and call, response stored in r, json passed to
    # template in context var
    tinysong_api_key = "da4d9bf5ee352d1f99eef3b73a8601cf"
    tinysong_url = "http://www.tinysong.com/s/"
    api_params = { 'key':tinysong_api_key, 'format':'json', 'limit':10 }
    r = requests.get(tinysong_url + search_string.replace(' ', '+'), params=api_params)
    context = {'tiny_json':r.json()}
    return render(request, 'songs/searchResults.html', context)

# get a req for next song(s), reply with top 5 song IDs
# request will come from rpi
def provideNextSong(request): 

    # a simple layer of authentication, need key=team2 in GET
    if 'key' in request.GET:
        if request.GET['key'] != "team2":
            raise Http404
    else:
        raise Http404

    # assemble top 5 song IDs (need string data)
    codes = []
    for song in Song.objects.order_by('-votes')[:5]: # we build a list of tinysong IDs for top 5
        codes += [str(song.id_code)]

    # if we're doing a 'pull' (meaning we are about to play the song)
    # then kill votes down to 0 for top song
    if 'pull' in request.GET:
        if request.GET['pull'] == "True": # this is probably a little bit bad practice (GET doing modification)
            s = Song.objects.order_by('-votes')[0]
            print "resetting votes for song ", s.title
            s.votes = 0
            s.save()
            
    return HttpResponse(','.join(codes) , content_type = "text/plain")
    

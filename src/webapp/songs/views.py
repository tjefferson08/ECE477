from django.shortcuts import render, get_object_or_404
from songs.models import Song
from django.http import HttpResponseRedirect, HttpResponse
from django.core.urlresolvers import reverse
from django.views import generic
from django.utils import timezone
import requests

# Create your views here.
class IndexView(generic.ListView):
    model = Song
    template_name = 'songs/index.html'
    context_object_name = 'latest_song_list'
    def get_queryset(self):
        return Song.objects.order_by('-title')[:5]

class DetailView(generic.DetailView):
    model = Song
    template_name = 'songs/detail.html'
    def get_queryset(self):
        return Song.objects.order_by('title')

class ResultsView(generic.DetailView):
    model = Song
    template_name = 'songs/results.html'
    def get_queryset(self):
        return Song.objects.order_by('title')

def vote(request, poll_id):
    p = get_object_or_404(Song, pk=poll_id)
    p.votes += 1
    p.save()

    # Always return an HttpResponseRedirect after successfully dealing
    # with POST data. This prevents data from being posted twice if a
    # user hits the Back button.
    return HttpResponseRedirect(reverse('songs:results', args=(p.id,)))

# we call this function by a button press on index.html
def searchSong(request):
    search_string = request.POST['title']

    # default search, if request left empty
    if not search_string:
        search_string = "Never gonna give you up"
    print search_string

    # api concatenation
    tinysong_api_key = "da4d9bf5ee352d1f99eef3b73a8601cf"
    tinysong_url = "http://www.tinysong.com/s/"
    api_params = { 'key':tinysong_api_key, 'format':'json', 'limit':10 }
    r = requests.get(tinysong_url + search_string.replace(' ', '+'), params=api_params)
    print r.url
    print r.text
    print r.encoding
    return render(request, 'songs/searchResults.html')
#    return HttpResponse(r.text)

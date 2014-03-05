from django.shortcuts import render, get_object_or_404
from polls.models import Poll, Choice
from django.http import HttpResponseRedirect, HttpResponse
from django.core.urlresolvers import reverse
from django.views import generic
from django.utils import timezone
import requests

# Create your views here.
class IndexView(generic.ListView):
    model = Poll
    template_name = 'polls/index.html'
    context_object_name = 'latest_poll_list'
    def get_queryset(self):
        return Poll.objects.filter(pub_date__lte=timezone.now()).order_by('-pub_date')[:5]

class DetailView(generic.DetailView):
    model = Poll
    template_name = 'polls/detail.html'
    def get_queryset(self):
        return Poll.objects.filter(pub_date__lte=timezone.now())

class ResultsView(generic.DetailView):
    model = Poll
    template_name = 'polls/results.html'
    def get_queryset(self):
        return Poll.objects.filter(pub_date__lte=timezone.now())

def vote(request, poll_id):
    p = get_object_or_404(Poll, pk=poll_id)
    try:
        selected_choice = p.choice_set.get(pk=request.POST['choice'])
    except (KeyError, Choice.DoesNotExist):
        # Redisplay the poll voting form.
        return render(request, 'polls/detail.html', {
            'poll': p,
            'error_message': "You didn't select a choice.",
        })
    else:
        selected_choice.votes += 1
        selected_choice.save()
        # Always return an HttpResponseRedirect after successfully dealing
        # with POST data. This prevents data from being posted twice if a
        # user hits the Back button.
        return HttpResponseRedirect(reverse('polls:results', args=(p.id,)))

def searchSong(request):
    search_string = request.POST['title']
    if not search_string:
        search_string = "Default search"
    print search_string
    tinysong_api_key = "da4d9bf5ee352d1f99eef3b73a8601cf"
    tinysong_url = "http://www.tinysong.com/s/"
    api_params = { 'key':tinysong_api_key, 'format':'json', 'limit':10 }
    r = requests.get(tinysong_url + search_string.replace(' ', '+'), params=api_params)
    print r.url
    return HttpResponse(r.text)

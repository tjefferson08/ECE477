from django.shortcuts import render, get_object_or_404
from polls.models import Poll, Choice
<<<<<<< HEAD
from django.core.urlresolvers import reverse
from django.http import HttpResponse, Http404, HttpResponseRedirect
=======
from django.http import HttpResponseRedirect, HttpResponse
from django.core.urlresolvers import reverse
from django.views import generic
>>>>>>> 2cf66cf9fa7a8995f645d7ee1392e3e71ace7558

# Create your views here.
class IndexView(generic.ListView):
    model = Poll
    template_name = 'polls/index.html'
    context_object_name = 'latest_poll_list'
    def get_queryset(self):
        return Poll.objects.order_by('-pub_date')[:5]

class DetailView(generic.DetailView):
    model = Poll
    template_name = 'polls/detail.html'

class ResultsView(generic.DetailView):
    model = Poll
    template_name = 'polls/results.html'

def vote(request, poll_id):
    p = get_object_or_404(Poll, pk=poll_id)
    try:
        selected_choice = p.choice_set.get(pk=request.POST['choice'])
    except (KeyError, Choice.DoesNotExist):
<<<<<<< HEAD
        # Redisplay the poll voting form
        return render(request, 'polls/detail.html', 
                      {'poll':p, 'error_message' : "You didn't select a choice.", })
    else:
        selected_choice.votes += 1
        selected_choice.save()
=======
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
>>>>>>> 2cf66cf9fa7a8995f645d7ee1392e3e71ace7558
        return HttpResponseRedirect(reverse('polls:results', args=(p.id,)))

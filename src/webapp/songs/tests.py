import datetime
from django.test import TestCase
from django.utils import timezone
from django.core.urlresolvers import reverse
from polls.models import Poll

def create_poll(question, days):
    return Poll.objects.create(question=question,
                               pub_date=timezone.now() + datetime.timedelta(days=days))

class PollMethodTests(TestCase):
    def test_was_published_recently_with_future_poll(self):
        # was_published_recently should return false for polls 
        # whose pub_date is in the future
        future_poll = Poll(pub_date=timezone.now() + datetime.timedelta(days=30))
        self.assertEqual(future_poll.was_published_recently(), False)
    
    def test_was_published_recently_with_old_poll(self):
        # was_pub_recently() should return true for polls with pub date within 1 day
        recent_poll = Poll(pub_date=timezone.now() - datetime.timedelta(days=30))
        self.assertEqual(recent_poll.was_published_recently(), False)

    def test_was_published_recently_with_recent_poll(self):
        # was_pub_recently() should return true for polls with pub date within 1 day
        recent_poll = Poll(pub_date=timezone.now() - datetime.timedelta(hours=1))
        self.assertEqual(recent_poll.was_published_recently(), True)
    
class PollViewTests(TestCase):
    def test_index_view_with_no_polls(self):
        response = self.client.get(reverse('polls:index'))
        self.assertEqual(response.status_code, 200)
        self.assertContains(response, "No polls are available.")
        self.assertQuerysetEqual(response.context['latest_poll_list'], [])

    def test_index_view_with_a_past_poll(self):
        create_poll(question="past poll", days=-30)
        response = self.client.get(reverse('polls:index'))
        self.assertQuerysetEqual(response.context['latest_poll_list'], ['<Poll: past poll>'])

    def test_index_view_with_a_future_poll(self):
        create_poll(question="Future poll.", days=30)
        response = self.client.get(reverse('polls:index'))
        self.assertContains(response, "No polls are available.", status_code=200)
        self.assertQuerysetEqual(response.context['latest_poll_list'], [])

    def test_index_view_with_future_poll_and_past_polls(self):
        create_poll(question="Future poll.", days=30)
        create_poll(question="past poll", days=-30)
        response = self.client.get(reverse('polls:index'))
        self.assertQuerysetEqual(response.context['latest_poll_list'], ['<Poll: past poll>'])

    def test_index_view_with_two_past_polls(self):
        create_poll(question="past poll.", days=-5)
        create_poll(question="past poll 2", days=-30)
        response = self.client.get(reverse('polls:index'))
        self.assertQuerysetEqual(response.context['latest_poll_list'], 
                                 ['<Poll: past poll.>', '<Poll: past poll 2>'])

class PollIndexDetailTests(TestCase):
    def test_detail_view_with_a_future_poll(self):
        future_poll = create_poll(question="Future poll.", days=5)
        response = self.client.get(reverse('polls:detail', args=(future_poll.id,)))
        self.assertEqual(response.status_code, 404)

    def test_detail_view_with_a_past_poll(self):
        past_poll = create_poll(question="Past poll.", days=-5)
        response = self.client.get(reverse('polls:detail', args=(past_poll.id,)))
        self.assertContains(response, past_poll.question, status_code=200)
                                   

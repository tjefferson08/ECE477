from django.conf.urls import patterns, url
from songs import views

urlpatterns = patterns('',
                       # ex: /songs/
                       url(r'^$', views.IndexView.as_view(), name='index'),
                       # ex: /songs/5/
                       url(r'^(?P<pk>\d+)/$', views.DetailView.as_view(), name='detail'),
                       # ex: /songs/5/results/
                       url(r'^(?P<pk>\d+)/results/$', views.ResultsView.as_view(), name='results'),
                       # ex: /songs/1234/vote
                       url(r'^(?P<song_id>\d+)/vote$', views.vote, name='vote'),
                       # ex: /songs/searchSong
                       url(r'^searchSong/$', views.searchSong, name='searchSong'),
                       )


from django.db import models
from django.utils import timezone
import datetime

# Create your models here.
class Song(models.Model):
    id_code = models.IntegerField()
    title = models.CharField(max_length=200)
    artist = models.CharField(max_length=200)
    album = models.CharField(max_length=200)
    votes = models.IntegerField()
    def __unicode__(self):
        return self.title

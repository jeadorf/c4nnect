#!/usr/bin/python

import cgi
import subprocess

def handle(request):
  p1 = subprocess.Popen(
      ["../build/server", request.command, request.board],
      stdout=subprocess.PIPE)
  return p1.communicate()[0]

class Request:
  def __init__(self):
    # extract GET parameters
    args = cgi.FieldStorage()
    self.command = args["command"].value
    self.board = args["board"].value
    # sanitize input
    assert self.command in ["get", "move"]
    assert len(self.board) < 32
    assert self.board.isdigit()

request = Request()
response = handle(request)
print "Content-type: application/json"
print
print response


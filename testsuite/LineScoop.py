#!/usr/bin/env python

import SocketServer, socket, sys, time

if len(sys.argv) < 3: print "Usage:", sys.argv[0], "FROMPORT TOPORT", sys.exit(1)

class LineGateway(SocketServer.BaseRequestHandler):

   def setup(self): # setup a connection to the peer
      for i in range(4): # retry to setup the connection
         try:
            self.peer = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.peer.connect(('localhost', int(sys.argv[1])))
            self.peer = self.peer.makefile()
            self.request = self.request.makefile()
            break
         except socket.error, e:
            if i is 3: raise # maximum retry count reached
            time.sleep(1)

   def finish(self): self.peer.close() # close connection to the peer

   def recvandsend(self, sock1, sock2): # get a line from sock1 and send it to sock2
      line = sock1.readline()
      if line is '': raise StopIteration # nothing received, sock1 closed connection
      sock2.write(line), sock2.flush()

   def handle(self): # do the actual echoing
      try:
         while True: # handle the connection until we caught an exception
            self.recvandsend(self.request, self.peer) # receive from request and send to peer
            self.recvandsend(self.peer, self.request) # receive from peer and send to request
      except Exception: pass # ignore exceptions, otw. self.finish() is not called

# start a threading TCPServer
class ThreadingTCPServer(SocketServer.ThreadingMixIn, SocketServer.TCPServer): pass
try: ThreadingTCPServer(('', int(sys.argv[2])), LineGateway).serve_forever()
except KeyboardInterrupt: pass


#!/usr/bin/env python
# return 0 -> ok
# return != 0 -> failed
import sys
import re

def parseAnswersetsToPythonSet(filename):
  f = open(filename,"r")
  ret = set()
  for line in f:
    atoms = re.findall("""
      [a-z"][a-zA-Z0-9"]*
      (?:
        \( (?: (?:[^")]+) | (?:"[^"]*") )+ \)
      )?
      """, line, re.X) # re.X means that whitespace in the regex is ignored

    #print 'line=', line, 'atoms=', atoms
    ret.add(frozenset(atoms))
  f.close()
  return ret

realOutput = sys.argv[1]
realOutputAS = parseAnswersetsToPythonSet(realOutput)
#print 'realOutputAS', realOutputAS

referenceOutput = sys.argv[2]
referenceOutputAS = parseAnswersetsToPythonSet(referenceOutput)
#print 'referenceOutputAS', referenceOutputAS

if (referenceOutputAS == realOutputAS):
	sys.exit(0) #true

# otherwise display differences

onlyInReference = referenceOutputAS - realOutputAS
onlyInReal = realOutputAS - referenceOutputAS

def displayProblematic(message,set):
  print "%s {%s}" % ( message, ", ".join(set) )
  #for s in set:
  #  print "hash=%d repr=%s" % (s.__hash__(), s.__repr__())

for s in onlyInReference:
  displayProblematic('missing answer set (only in reference file):', s)
for s in onlyInReal:
  displayProblematic('bad answer set (not in reference file):', s)

# only one answer set is missing and only one is too much -> support user  in debugging
if len(onlyInReference) == 1 and len(onlyInReal) == 1:
  ref = onlyInReference.pop()
  real = onlyInReal.pop()
  displayProblematic('single answer set difference missing atoms:', ref-real)
  displayProblematic('single answer set difference bad atoms:', real-ref)
  

sys.exit(-1) #false

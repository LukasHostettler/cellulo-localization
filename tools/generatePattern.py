#
#   Free Dot Toolkit : matching.py: utilities for handling dots and
#                                   generating postscript code.
#   Version: PR0; first prototype released to the public.
#   (c) Gopi Flaherty 2005 gf2e@andrew.cmu.edu
#   This software was first publicly released in Karlsruhe, Germany,
#   on June 25, 2005.
#
#     This file is part of the Free Dot Toolkit.
#     
#     The Free Dot Toolkit is free software; you can redistribute it
#     and/or modify it under the terms of the GNU General Public License
#     as published by the Free Software Foundation; either version 2
#     of the License, or (at your option) any later version.
#     
#     The Free Dot Toolkit is distributed in the hope that it will be
#     useful, but WITHOUT ANY WARRANTY; without even the implied
#     warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#     See the GNU General Public License for more details.
#     
#     You should have received a copy of the GNU General Public License
#     along with the Free Dot Toolkit; if not, write to the Free Software
#     Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

import argparse
dot_seq=[0,0,0,0,0,0,1,0,0,1,1,1,1,1,0,1,0,0,1,0,0,0,0,1,1,1,0,1,1,1,0,0,1,0,1,0,1,0,0,0,1,0,1,1,0,1,1,0,0,1,1,0,1,0,1,1,1,1,0,0,0,1,1]
#dot_seq = [0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0]
dot_val = [[0,3],[1,2]]#x,y
def getDotVal(xVal, yVal):
    """Return the numeric dot 0..3 for an xVal,yVal start position"""
    xBit = dot_seq[xVal]
    yBit = dot_seq[yVal]
    return dot_val[xBit][yBit]

class anotoSequence(type([])):
    def getDelta(self):
        """returns seq[n]-seq[n+1] mod 63 for each n"""
        ret = []
        for i in xrange(len(self)-1):
            ret.append((self[i]-self[i+1]) %63)
        return ret
    def addModulo(self, delta):
        for i in xrange(len(self)):
            self[i] = (self[i]+delta)%63
    def isMatch(self, myStart, othStart, oth):
        """Does the list 'oth' match?"""
        return self[myStart:myStart+10] == oth[othStart:othStart+10]
    def mergeList(self, myStart, oth):
        """Merge a list, with oth[0] going to position myStart."""
        if myStart>len(self):
            print "WARNING: mergeList is extending the list!"
            self += [-3]*(myStart-len(self))
        if myStart<0:
            print "bar ***"
            print len(self)
            print len(oth[:-myStart])
            self[0:0] = oth[:-myStart]#pre-pend the extra elements...
            if len(self)<len(oth):#append elements if needed...
                self.extend(oth[len(self):])
            for i in xrange(len(oth)+myStart):#remaining elements in the new list...
                print "foo"
                print len(self)
                print len(oth)
                print len(oth[len(self):])
                print i-myStart
                print i
                print myStart
                if self[i-myStart]<0:
                    self[i-myStart] = oth[i-myStart]
        else:
            for i in xrange(len(self)-myStart):#Start with overlap...
                if self[i+myStart]<0:
                    self[i+myStart] = oth[i]
            self.extend(oth[len(self)-myStart:])
    def __str__(self):
        return "a:"+super(anotoSequence,self).__str__()
    def __repr__(self):
        return "a:"+super(anotoSequence,self).__repr__()

class anotoEdge(object):
    def __init__(self,left,top):
        self.left = anotoSequence()
        self.left += left
        self.top = anotoSequence()
        self.top += top
    def prependLeft(self, elems):
        """We've got a properly shifted list to pre-pend to the left of the top list.
        Do apropriate modulo stuff to our left edge."""
        self.left.addModulo(-len(elems))
        ts = anotoSequence()
        ts += elems + self.top
        self.top = ts
    def prependAbove(self, elems):
        """Found some dots that belong above us, vertically..."""
        self.top.addModulo(-len(elems))#shift the top row up...
        ls = anotoSequence()
        ls += elems + self.left
        self.left = ls
    def appendRight(self, elems):
        self.top += elems
    def appendBottom(self, elems):
        self.left += elems
    def mergeEdges(self, xPos, yPos, oth):
        """The (left,top) of the anotoEdge oth is at xPos, yPos; join us!"""
        if xPos>=0:
            #top: modulo-shift new row and then merge it.
            oth.top.addModulo(-yPos)
            self.top.mergeList(xPos, oth.top)
        else:
            #top: modulo shift our existing row and merge.
            self.top.addModulo(yPos)
            self.top.mergeList(xPos, oth.top)
        if yPos>=0:
            #left: modulo-shift new row and merge...
            oth.left.addModulo(-xPos)
            self.left.mergeList(yPos, oth.left)
        else:
            #left: modulo-shift our row and merge...
            self.left.addModulo(xPos)
            self.left.mergeList(yPos, oth.left)
        return
        if xPos>=0 and yPos>=0:#new stuff is down, right
            #top: modulo-shift it up and then merge it.
            oth.top.addModulo(-yPos)
            self.top.mergeList(xPos, oth.top)
            #left: modulo-shift it and merge...
            oth.left.addModulo(-xPos)
            self.left.mergeList(yPos, oth.left)
        elif xPos >= 0 and yPos <=0:#new stuff is up, right
            #top: modulo shift our existing row and merge.
            self.top.addModulo(yPos)
            self.top.mergeList(xPos, oth.top)
            #left: modulo-shift the new thing and merge.
            oth.left.addModulo(-xPos)
            self.left.mergeList(yPos, oth.left)
        elif xPos <= 0 and yPos >=0:#new stuff is down, left
            #top: modulo shift new row and merge.
            self.top.addModulo(yPos)
            self.top.mergeList(xPos, oth.top)
            #left: modulo-shift the new thing and merge.
            oth.left.addModulo(-xPos)
            self.left.mergeList(yPos, oth.left)
    def genPage(self, name="match_out.ps", title="unknown page", width=0.3,offset=1.2):
        scalefactor=width/0.3;
        f=open(name,'w')
        f.write("""%!PS-Adobe-3.0
%%Creator: Hostettler Lukas
%%CreationDate: Fri Mar 13 15:13:50 2015
%%Pages: 1
%%DocumentData: Clean7Bit
%%LanguageLevel: 2
%%DocumentMedia: A4 595 842 0 () ()
%%BoundingBox: 0 841 0 842
%%EndComments
<</PageSize[595 842]/ImabingBBox null>>setpagedevice
<</HWResolution[1200 1200]>>setpagedevice
/Helvetica findfont 20 scalefont setfont\n""")
        f.write("110 620 moveto ("+title+") show\n")
        f.write("""8 dict begin
  /FontType 3 def
  /FontMatrix [ .001 0 0 .001 0 0] def
  /FontMatrix [ 1 0 0 1 0 0] def
  /FontBBox [0 0 75 75] def
  /Encoding 256 array def
  0 1 255 {Encoding exch /.notdef put} for
  Encoding 48 /upDot put
  Encoding 49 /rightDot put
  Encoding 50 /downDot put
  Encoding 51 /leftDot put
  Encoding 97 /square put
  Encoding 98 /triangle put
  /CharProcs 3 dict def
  CharProcs begin
    /.notdef {} def
    /square
    { 0 0 moveto
      75 0 lineto
      75 75 lineto
      0 75 lineto
      closepath
      fill
    } bind def
    /triangle
    { 0 0 moveto
      37.5 75 lineto
      75 0 lineto
      closepath
      fill
    } bind def
    /upDot
    {
      0 """+str(offset)+""" 0.07 0 360 arc
      closepath fill
    } bind def
    /downDot
    {
      0 -"""+str(offset)+""" 0.07 0 360 arc
      closepath fill
    } bind def
    /leftDot
    {
      -"""+str(offset)+""" 0 0.07 0 360 arc
      closepath fill
    } bind def
    /rightDot
    {
      """+str(offset)+""" 0 0.07 0 360 arc
      closepath fill
    } bind def
  end
  /BuildGlyph
  {0.0118 72 mul 0   %width
    -1 -1 1 1 %bounding box
    setcachedevice
    exch /CharProcs get exch
    2 copy known not
    {pop /.notdef}
    if
    get exec
  } bind def
  /BuildChar
  {1 index /Encoding get exch get
    1 index /BuildGlyph get exec
  } bind def
  currentdict
end
/ExampleFont exch definefont pop
/ExampleFont findfont """)
	f.write(str(scalefactor));
	f.write(""" scalefont setfont\n""")
        for y in xrange(len(self.left)):
            f.write("30 "+str(800-scalefactor*y*0.0118*72)+" moveto\n(")
            for x in xrange(len(self.top)):
                f.write(str(getDotVal(
                            (self.left[y]+x) %63,
                            (self.top[x] +y) %63,
                            )))
            f.write(") show\n")
        f.write("showpage\n")
    def __str__(self):
        return `self`
    def __repr__(self):
        return "e:"+`(self.left,self.top)`

def testMod():
    ea = anotoEdge([0,1,2,3],[0,1,2,3])
    eb = anotoEdge([2,3,4,5],[2,3,4,5])
    ec = anotoEdge([0,1,2,3],[0,1,2,3])
#     eb = anotoEdge([0,1,2,3,4,5],[0,1,-2])
#     ec = anotoEdge([4,5,6,7,8,9],[66,66,66])
    print ea
    ea.mergeEdges(1,1,eb)
    print ea
    ea.mergeEdges(-1,-1,ec)
    print ea

def delta(li):
    ret = []
    for i in xrange(len(li)-1):
        ret.append((li[i]-li[i+1])%63)
    return ret
def add(li,delta=32):
    ret = []
    for i in li:
        ret.append((i+delta)%63)
    return ret

def a():
    print c_vert
    print delta(c_vert)
    print delta(add(c_vert))


a1=[0,0,0,0,0,1,0,0,0,0,2,0,1,0,0,1,0,1,0,0,2,0,0,0,1,1,0,0,0,1,2,0,0,1,0,2,0,0,2,0,2,0,1,1,0,1,0,1,1,0,2,0,1,2,0,1,0,1,2,0,2,1,0,0,1,1,1,0,1,1,1,1,0,2,1,0,1,0,2,1,1,0,0,1,2,1,0,1,1,2,0,0,0,2,1,0,2,0,2,1,1,1,0,0,2,1,2,0,1,1,1,2,0,2,0,0,1,1,2,1,0,0,0,2,2,0,1,0,2,2,0,0,1,2,2,0,2,0,2,2,1,0,1,2,1,2,1,0,2,1,2,1,1,0,2,2,1,2,1,2,0,2,2,0,2,2,2,0,1,1,2,2,1,1,0,1,2,2,2,2,1,2,0,0,2,2,1,1,2,1,2,2,1,0,2,2,2,2,2,0,2,1,2,2,2,1,1,1,2,1,1,2,0,1,2,2,1,2,2,0,1,2,1,1,1,1,2,2,2,0,0,2,1,1,2,2]
a2=[0,0,0,0,0,1,0,0,0,0,2,0,1,0,0,1,0,1,0,1,1,0,0,0,1,1,1,1,0,0,1,1,0,1,0,0,2,0,0,0,1,2,0,1,0,1,2,1,0,0,0,2,1,1,1,0,1,1,1,0,2,1,0,0,1,2,1,2,1,0,1,0,2,0,1,1,0,2,0,0,1,0,2,1,2,0,0,0,2,2,0,0,1,1,2,0,2,0,0,2,0,2,0,1,2,0,0,2,2,1,1,0,0,2,1,0,1,1,2,1,0,2,0,2,2,1,0,0,2,2,2,1,0,1,2,2,0,0,2,1,2,2,1,1,1,1,1,2,0,0,1,2,2,1,2,0,1,1,1,2,1,1,2,0,1,2,1,1,1,2,2,0,2,2,0,1,1,2,2,2,2,1,2,1,2,2,0,1,2,2,2,0,2,0,2,1,1,2,2,1,0,2,2,0,2,1,0,2,1,1,0,2,2,2,2,0,1,0,2,2,1,2,2,2,1,1,2,1,2,0,2,2,2]
a3=[0,0,0,0,0,1,0,0,1,1,0,0,0,1,1,1,1,0,0,1,0,1,0,1,1,0,1,1,1,0,1]
a4=[0,0,0,0,0,1,0,2,0,0,0,0,2,0,0,2,0,1,0,0,0,1,1,2,0,0,0,1,2,0,0,2,1,0,0,0,2,1,1,2,0,1,0,1,0,0,1,2,1,0,0,1,0,0,2,2,0,0,0,2,2,1,0,2,0,1,1,0,0,1,1,1,0,1,0,1,1,0,1,2,0,1,1,1,1,0,0,2,0,2,0,1,2,0,2,2,0,1,0,2,1,0,1,2,1,1,0,1,1,1,2,2,0,0,1,0,1,2,2,2,0,0,2,2,2,0,1,2,1,2,0,2,0,0,1,2,2,0,1,1,2,1,0,2,1,1,0,2,0,2,1,2,0,0,1,1,0,2,1,2,1,0,1,0,2,2,0,2,1,0,2,2,1,1,1,2,0,2,1,1,1,0,2,2,2,2,0,2,0,2,2,1,2,1,1,1,1,2,1,2,1,2,2,2,1,0,0,2,1,2,2,1,0,1,1,2,2,1,1,2,1,2,2,2,2,1,2,0,1,2,2,1,2,2,0,2,2,2,1,1,1]
def generateDelta(startx=0,length=100):
	delta=[]
	for i in xrange(length):
		delta.append(calculateDeltaFromPos(startx+i))
	return delta   
def generatePrimaryOffset(delta, startval=0):
	ans=[startval]
	for i in xrange(len(delta)):
		#print(str(i));
		ans.append( (ans[i]+delta[i])%63)
	return ans
def calculateDeltaFromPos(pos):
	ans=5+a1[pos%len(a1)]+3*a2[pos%len(a2)]+9*a3[pos%len(a3)]+18*a4[pos%len(a4)]
	return ans


def testPS(debug=1,width=0.5,x0=0,y0=1000,offset=0.12,page_width=0,page_height=0):
    num_hor=int(190/width)
    num_ver=int(270/width)
    delta=generateDelta(startx=x0,length=num_hor)
    seq_top=generatePrimaryOffset(delta)
    delta=generateDelta(startx=y0,length=num_ver)
    seq_left=generatePrimaryOffset(delta);
    #print(seq_left)
    e = anotoEdge(seq_left, seq_top)
    info=""
    if debug:
    	info="spacing:"+str(width)+" mm, x0="+str(x0)+",y0="+str(y0) +",offset="+str(offset)
    e.genPage(name="a4page1.ps", title=info,width=width,offset=offset)
    #e = anotoEdge(savenotes_left, savenotes_top)
    #e.genPage(name="a4savenotes.ps", title="a4 Save Notes",width=1)
    
def comp():
    chd = delta(e_horiz)
    bhd = delta(d_horiz)
    print len(chd),len(bhd)
#    print chd
#    print bhd
#    chd = range(50)
#    bhd = range(20,70)
    for i in xrange(1,len(chd)):
        if bhd[i:i+10]==chd[15:25]:
            print "0 got it:"+str(i)
            delt = b_horiz[i] - c_horiz[0]
            print "delta : "+str(delt)
            return b_horiz[0:i]+add(c_horiz,delta=delt)
        elif chd[i:i+10]==bhd[0:10]:
            print "1 got it:"+str(i)
        elif bhd[-i:10]==chd[10:i]:
            print "2 got it:"+str(i)
        elif chd[-i:10]==bhd[10:i]:
            print "3 got it:"+str(i)
        else:
#            print `(chd[i:i+10],bhd[0:10])`
            pass
# >>> m.b_horiz[460:470]
# [35, 47, 7, 34, 47, 42, 10, 53, 29, 45]
# >>> m.c_horiz[0:25]
# [34, 46, 6, 33, 46, 41, 9, 52, 28, 44, 10, 18, 1, 59, 17, 8, 42, 54, 40, 29, 59, 31, 56, 51, 7]
# >>> m.b_horiz[450:460] 
# [50, 31, 5, 45, 58, 51, 9, 0, 31, 23]
#[28, 39, 13, 5, 59, 18, 48, 10, 3, 34, 50, 31, 5, 45, 58, 51, 9, 0, 31, 23, 35, 47, 7, 34, 47, 42, 10, 53, 29, 45]

#chd = b_horiz, bhd = d_horiz 0 got it at 14, delta 17, chd[15:27]
#chd = b_vert, bhd = d_vert, 0 got it at 16, delta -13
#chd = e_vert, bhd = d_vert, 0 got it at 180, delta 9, chd[15:25]
#chd = e_horiz, bhd=d_horiz, 0 got it at 12, delta 15, chd[15:25]
if __name__=='__main__':
	parser = argparse.ArgumentParser(description='Generates the patterns used for the cellulo-project')
	parser.add_argument("-g","--gridspacing",default=0.6,type=float,help='The spacing between the dots in mm')
	parser.add_argument("-x","--x-left",default=0,type=int,help='x coordinate of the left side')
	parser.add_argument("-y","--y-top",default=0,type=int,help='y coordinate of the top')
	#group = parser.add_mutually_exclusive_group(); 
	#group.add_argument("-f","--paper-format",default='A4', type=str,choices=['A0','A1','A2','A3','A4','A5'],help='format of the paper')
	#group.add_argument("-c","--custom-format",nargs=2,metavar=("WIDTH", "HEIGHT"),type=float,help='custom format WIDTH HEIGH in mm')
	parser.add_argument("-o","--dot-offset",default=0.145, type=float,help='ratio between the dot offset and the gridspacing')
	#parser.add_argument("-s","--dot-size", default=1.0, type=float,help='ratio between radius of the dot and the gridspacing')
	parser.add_argument("-d","--debug", action='store_true',help='prints the infomation needed for generation on pattern')
	args=parser.parse_args();
	testPS(width=args.gridspacing,offset=args.dot_offset,y0=args.y_top,x0=args.x_left,debug=args.debug)
#    comp()
#    testMod()
#    print comp()[440:470]

import ROOT as R
gStyle = R.gStyle
from itertools import product

R.gROOT.SetBatch()
## Style
gStyle.SetLineWidth(1)
gStyle.SetTextSize(1.1);
gStyle.SetLabelSize(0.045, "xy")
gStyle.SetTitleSize(0.06, "xy")
gStyle.SetTitleOffset(0.9, "x")
gStyle.SetTitleOffset(1.2, "y")

gStyle.SetPadTopMargin(0.13)
gStyle.SetPadRightMargin(0.05)
gStyle.SetPadBottomMargin(0.15)
gStyle.SetPadLeftMargin(0.15)
gStyle.SetPaperSize(28,20)

gStyle.SetOptStat(1110)
gStyle.SetStatBorderSize(1)
gStyle.SetStatFontSize(0.04)
gStyle.SetStatW(0.25)
gStyle.SetStatH(0.2)
gStyle.SetStatX(0.95)
gStyle.SetStatY(0.99)

myfile = R.TFile("osc_channels_1.01-12.01.root")
keys = myfile.GetListOfKeys()

ntuples = (key.GetName() for key in keys)
ntuples = [(key,myfile.Get(key)) for key in ntuples]

can = R.TCanvas()
can.Divide(3,2)

branches = [branch.GetName() for branch in ntuples[0][1].GetListOfBranches()]
branches.sort()
del branches[-1]


print branches

for n, tup in enumerate(product(ntuples, branches), start=1):
    i, j = tup
    pad = can.cd(n)
    name = i[0]
    tree = i[1]
    print name, j
    #tree.Draw(j,"%s > 15" % j, "")
    tree.Draw(j)
    histo = pad.GetPrimitive("htemp")
    histo.Rebin(2)
    histo.SetTitle("{} {};Amplitude (V);Counts".format(j.upper(),name.capitalize()))
    pad.SetLogy()
    pad.Update()

can.Update()
can.Print("oscilloscope.pdf")
#key.Draw(next(branches))


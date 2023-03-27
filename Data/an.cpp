#include <iomanip>
#include <iostream>
#include <math.h>

#include <TAxis.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TLegend.h>

int main() {

  TGraph *graph_S = new TGraph("Result.txt", "%lg %lg");

  graph_S->SetTitle(0);
  graph_S->SetMarkerSize(0);
  graph_S->SetLineColor(kGray + 2);
  graph_S->SetLineWidth(2);
  //graph_S->GetYaxis()->SetRangeUser(0., population + 1000);

  //TLegend *leg1 = new TLegend(.735, .850, .897, .750);
  //leg1->SetFillColor(0);
  //leg1->AddEntry(graph_S, "Susceptible");
  //leg1->AddEntry(graph_I, "Infected");
  //leg1->AddEntry(graph_D, "Dead");


  TCanvas *canva1 = new TCanvas("c1", "", 1920, 1080);
  canva1->SetGrid();
  graph_S->Draw("AC");
  //leg1->Draw("same");
  canva1->Print("Graph.pdf");

  //TCanvas *canva2 = new TCanvas("c1", "", 1920, 1080);
  //canva2->SetGrid();
  //graph_Sr->Draw("AC");
  //graph_Ir->Draw("C Same");
  //graph_Dr->Draw("C Same");
  //leg2->Draw("same");
  //canva2->Print("Data/GraphCut.pdf");
}
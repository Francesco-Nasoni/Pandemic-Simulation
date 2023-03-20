#include <iomanip>
#include <iostream>
#include <math.h>

#include <TAxis.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TLegend.h>

int main() {
  double cut = 600;
  int population = 10000;

  TGraph *graph_S = new TGraph("Data/Result.txt", "%lg %lg");
  TGraph *graph_I = new TGraph("Data/Result.txt", "%lg %*s %lg");
  TGraph *graph_D = new TGraph("Data/Result.txt", "%lg %*s %*s %lg");

  TGraph *graph_Sr = new TGraph("Data/Result.txt", "%lg %lg");
  TGraph *graph_Ir = new TGraph("Data/Result.txt", "%lg %*s %lg");
  TGraph *graph_Dr = new TGraph("Data/Result.txt", "%lg %*s %*s %lg");

  graph_S->SetTitle(0);
  graph_S->SetMarkerSize(0);
  graph_S->SetLineColor(kGray + 2);
  graph_S->SetLineWidth(2);
  graph_S->GetYaxis()->SetRangeUser(0., population + 1000);

  graph_I->SetTitle(0);
  graph_I->SetMarkerSize(0);
  graph_I->SetLineColor(kAzure);
  graph_I->SetLineWidth(2);
  graph_I->GetYaxis()->SetRangeUser(0., population + 1000);

  graph_D->SetTitle(0);
  graph_D->SetMarkerSize(0);
  graph_D->SetLineColor(kRed);
  graph_D->SetLineWidth(2);
  graph_D->GetYaxis()->SetRangeUser(0., population + 1000);

  graph_Sr->SetTitle(0);
  graph_Sr->SetMarkerSize(0);
  graph_Sr->SetLineColor(kGray + 2);
  graph_Sr->SetLineWidth(2);
  graph_Sr->GetXaxis()->SetRangeUser(0., cut);
  graph_Sr->GetYaxis()->SetRangeUser(0., population + 1000);

  graph_Ir->SetTitle(0);
  graph_Ir->SetMarkerSize(0);
  graph_Ir->SetLineColor(kAzure);
  graph_Ir->SetLineWidth(2);
  graph_Ir->GetXaxis()->SetRangeUser(0., cut);
  graph_Ir->GetYaxis()->SetRangeUser(0., population + 1000);

  graph_Dr->SetTitle(0);
  graph_Dr->SetMarkerSize(0);
  graph_Dr->SetLineColor(kRed);
  graph_Dr->SetLineWidth(2);
  graph_Dr->GetXaxis()->SetRangeUser(0., cut);
  graph_Dr->GetYaxis()->SetRangeUser(0., population + 1000);

  TLegend *leg1 = new TLegend(.735, .850, .897, .750);
  leg1->SetFillColor(0);
  leg1->AddEntry(graph_S, "Susceptible");
  leg1->AddEntry(graph_I, "Infected");
  leg1->AddEntry(graph_D, "Dead");

  TLegend *leg2 = new TLegend(.735, .850, .897, .750);
  leg2->SetFillColor(0);
  leg2->AddEntry(graph_S, "Susceptible");
  leg2->AddEntry(graph_I, "Infected");
  leg2->AddEntry(graph_D, "Dead");

  TCanvas *canva1 = new TCanvas("c1", "", 1920, 1080);
  canva1->SetGrid();
  graph_S->Draw("AC");
  graph_I->Draw("C Same");
  graph_D->Draw("C Same");
  leg1->Draw("same");
  canva1->Print("Data/Graph.pdf");

  TCanvas *canva2 = new TCanvas("c2", "", 1920, 1080);
  canva2->SetGrid();
  graph_Sr->Draw("AC");
  graph_Ir->Draw("C Same");
  graph_Dr->Draw("C Same");
  leg2->Draw("same");
  canva2->Print("Data/GraphCut.pdf");
}
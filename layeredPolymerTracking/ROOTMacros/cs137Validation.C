{
    // ROOT Macro for analyzing histogram data

    // Getting the files
    TFile *f25 = new TFile("cs137_25um.root");
    TFile *f50 = new TFile("cs137_50um.root");
    TFile *f100 = new TFile("cs137_100um.root");
    TFile *f1mm = new TFile("cs137_1mm.root");
    TFile *f1cm = new TFile("cs137_1cm.root");

    // Getting the Histograms
    TH1F* h25 = (TH1F*) f25->Get("totEventEDepGap_00");
    TH1F* h50 = (TH1F*) f50->Get("totEventEDepGap_00");
    TH1F* h100 = (TH1F*) f100->Get("totEventEDepGap_00");
    TH1F* h1mm = (TH1F*) f1mm->Get("totEventEDepGap_00");
    TH1F* h1cm = (TH1F*) f1cm->Get("totEventEDepGap_00");

    // Plotting
    gStyle->SetOptStat(0);
    h25->Draw();
    h50->Draw("same");
    h100->Draw("same");
    h1mm->Draw("same");
    h1cm->Draw("same");
  
    // Getting the number of entries
    fprintf(stdout,"25 um has %5.4e entries\n",(h25->GetEntries()-h25->GetBinContent(1)));
    fprintf(stdout,"50 um has %5.4e entries\n",(h50->GetEntries()-h50->GetBinContent(1)));
    fprintf(stdout,"100 um has %5.4e entries\n",(h100->GetEntries()-h100->GetBinContent(1)));
    fprintf(stdout,"1 mm has %5.4e entries\n",(h1mm->GetEntries()-h1mm->GetBinContent(1)));
    fprintf(stdout,"1 cm has %5.4e entries\n",(h1cm->GetEntries()-h1cm->GetBinContent(1)));
    
    // Scaling
    int numEvents = 1000000000;
    float scale = 1.0/(float)numEvents;
    h25->Scale(scale);
    h50->Scale(scale);
    h100->Scale(scale);
    h1mm->Scale(scale);
    h1cm->Scale(scale);

    // Plot Refinement
    h50->SetLineColor(2);       // Red
    h100->SetLineColor(3);      // Light Green
    h1mm->SetLineColor(4);      // Blue
    h1cm->SetLineColor(5);      // Yellow

    TAxis *xaxis = h25->GetXaxis();
    xaxis->SetRangeUser(0,0.67);
    
    TLegend* leg = new TLegend(0.8,0.7,0.9,0.9);
    leg->AddEntry(h25,"25 um","l");
    leg->AddEntry(h50,"50 um","l");
    leg->AddEntry(h100,"100 um","l");
    leg->AddEntry(h1mm,"1 mm","l");
    leg->AddEntry(h1cm,"1 cm","l");
    leg->Draw("same");

    h25->GetXaxis()->SetTitle("Energy Deposition per Event (MeV)");
    h25->GetYaxis()->SetTitle("Frequency");
    h25->SetTitle("Cs-137 Energy Deposition");
    
    gPad->SetLogy();

    // Energy Deposition
    fprintf(stdout,"\nMean \n");
    fprintf(stdout,"25 um has %5.4e MeV deposited\n",h25->GetMean());
    fprintf(stdout,"50 um has %5.4e MeV deposited\n",h50->GetMean());
    fprintf(stdout,"100 um has %5.4e MeV deposited\n",h100->GetMean());
    fprintf(stdout,"1 mm has %5.4e MeV deposited\n",h1mm->GetMean());
    fprintf(stdout,"1 cm has %5.4e MeV deposited\n",h1cm->GetMean());
    
    fprintf(stdout,"\nIntegral \n");
    fprintf(stdout,"25 um has %5.4e MeV deposited\n",h25->Integral("width"));
    fprintf(stdout,"50 um has %5.4e MeV deposited\n",h50->Integral("width"));
    fprintf(stdout,"100 um has %5.4e MeV deposited\n",h100->Integral("width"));
    fprintf(stdout,"1 mm has %5.4e MeV deposited\n",h1mm->Integral("width"));
    fprintf(stdout,"1 cm has %5.4e MeV deposited\n",h1cm->Integral("width"));
}

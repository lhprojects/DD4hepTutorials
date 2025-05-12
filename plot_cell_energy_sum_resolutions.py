import ROOT
from podio import root_io
import os
import matplotlib.pyplot as plt

ROOT.gROOT.SetBatch(ROOT.kTRUE)

means = []
sigmas = []
ratios = []
indices = []

for i in range(1, 10):
    input_file = f"simplecalo_{i}.root"
    if not os.path.exists(input_file):
        print(f"Missing: {input_file}")
        continue

    # Open ROOT file using podio
    reader = root_io.Reader(input_file)

    # Histogram for cell energy sum
    hist = ROOT.TH1F("cell_energy_sum_th1", ";Cell energy sum [GeV]; Number of events", 150, 0, 15.0)

    for event in reader.get("events"):
        calo_cells = event.get("simplecaloRO")
        total_energy = sum(cell.getEnergy() for cell in calo_cells)
        hist.Fill(total_energy)

    # Fit
    canvas = ROOT.TCanvas("cell_energy_sum_canvas")
    fit_min = hist.GetXaxis().GetBinCenter(hist.GetMaximumBin()) - 3 * hist.GetRMS()
    fit_max = hist.GetXaxis().GetBinCenter(hist.GetMaximumBin()) + 3 * hist.GetRMS()
    hist.Fit("gaus", "SQ", "", fit_min, fit_max)

    fit_result = hist.GetFunction("gaus")
    if fit_result:
        mean = fit_result.GetParameter(1)
        sigma = fit_result.GetParameter(2)

        means.append(mean)
        sigmas.append(sigma)
        ratios.append(sigma / mean if mean != 0 else 0)
        indices.append(i)

        # Save histogram as PNG
        ROOT.gStyle.SetOptFit(1111)
        hist.Draw()
        canvas.Print(f"simplecalo_{i}_cell_energy_sum.png")

    else:
        print(f"Fit failed for {input_file}")

# Plot sigma/mean vs i
plt.figure(figsize=(5, 5))
plt.plot(indices, ratios, 'o-', label="σ / μ")
plt.xlabel("Sensitive layer thickness (cm)")
plt.ylabel("Relative Spread (σ / μ)")
plt.title("Energy Spread vs Layer Thickness")
plt.grid(True)
plt.tight_layout()
plt.savefig("summary_sigma_over_mean.png")
plt.show()



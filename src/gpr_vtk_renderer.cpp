#include "gpr_vtk_renderer.h"
#include "gpr_dataset.h"

#include <vtkImageData.h>
#include <vtkImageSliceMapper.h>
#include <vtkImageSlice.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkInteractorStyleImage.h>
#include <vtkNew.h>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2)
VTK_MODULE_INIT(vtkInteractionStyle)

void GprVtkRenderer::render(const GprDataset& dataset) {
    const auto& traces = dataset.traces;
    const int numTraces = static_cast<int>(traces.size());
    if (numTraces == 0) return;
    const int numSamples = static_cast<int>(traces[0].samples.size());
    if (numSamples == 0) return;

    // Find global min/max for normalization
    float vmin = traces[0].samples[0];
    float vmax = vmin;
    for (const auto& trace : traces) {
        for (float s : trace.samples) {
            if (s < vmin) vmin = s;
            if (s > vmax) vmax = s;
        }
    }
    float range = vmax - vmin;
    if (range == 0.f) range = 1.f;

    // Build vtkImageData: X = samples, Y = traces
    vtkNew<vtkImageData> image;
    image->SetDimensions(numSamples, numTraces, 1);
    image->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

    auto* ptr = static_cast<unsigned char*>(image->GetScalarPointer());
    for (int t = 0; t < numTraces; ++t) {
        for (int s = 0; s < numSamples; ++s) {
            float normalized = (traces[t].samples[s] - vmin) / range;
            ptr[t * numSamples + s] = static_cast<unsigned char>(normalized * 255.f);
        }
    }

    // Grayscale lookup table
    vtkNew<vtkLookupTable> lut;
    lut->SetRange(0, 255);
    lut->SetValueRange(0.0, 1.0);
    lut->SetSaturationRange(0.0, 0.0);
    lut->SetHueRange(0.0, 0.0);
    lut->SetNumberOfTableValues(256);
    lut->Build();

    vtkNew<vtkImageMapToColors> colors;
    colors->SetInputData(image);
    colors->SetLookupTable(lut);
    colors->Update();

    vtkNew<vtkImageSliceMapper> sliceMapper;
    sliceMapper->SetInputConnection(colors->GetOutputPort());

    vtkNew<vtkImageSlice> slice;
    slice->SetMapper(sliceMapper);

    vtkNew<vtkRenderer> renderer;
    renderer->AddViewProp(slice);
    renderer->ResetCamera();
    renderer->SetBackground(0.1, 0.1, 0.1);

    vtkNew<vtkRenderWindow> window;
    window->AddRenderer(renderer);
    window->SetWindowName("GPR B-scan");
    window->SetSize(1000, 600);

    vtkNew<vtkInteractorStyleImage> style;
    vtkNew<vtkRenderWindowInteractor> interactor;
    interactor->SetInteractorStyle(style);
    interactor->SetRenderWindow(window);

    window->Render();
    interactor->Start();
}

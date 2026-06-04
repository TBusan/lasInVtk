#pragma once

struct GprDataset;

class GprVtkRenderer {
public:
    void render(const GprDataset& dataset);
};

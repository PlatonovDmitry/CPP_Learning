#pragma once

#include "entities.h"
#include "summing_segment_tree.h"

#include <cstdint>
#include <cmath>

struct BulkMoneyAdder {
    double delta = {};
};

struct BulkMoneySpender {
    double delta = {};
};

struct BulkTaxApplier {
    double ComputeFactor() const {
        return factor;
    }

    double factor = 1;
};

class BulkLinearUpdater {
public:
    BulkLinearUpdater() = default;

    BulkLinearUpdater(const BulkMoneyAdder& add)
        : add_(add) {
    }

    BulkLinearUpdater(const BulkTaxApplier& tax)
        : tax_(tax) {
    }

    BulkLinearUpdater(const BulkMoneySpender& spend)
        : spend_(spend) {
    }

    void CombineWith(const BulkLinearUpdater& other) {
        tax_.factor *= other.tax_.factor;
        add_.delta = add_.delta * other.tax_.ComputeFactor() + other.add_.delta;
        spend_.delta += other.spend_.delta;
    }

    DayBudget Collapse(DayBudget origin, IndexSegment segment) const {
        DayBudget output;
        output.income = origin.income * tax_.ComputeFactor() + add_.delta * static_cast<double>(segment.length());
        output.spend = origin.spend + spend_.delta * static_cast<double>(segment.length());
        return output;
    }

private:
    // apply tax first, then add
    BulkTaxApplier tax_;
    BulkMoneyAdder add_;
    BulkMoneySpender spend_;
};

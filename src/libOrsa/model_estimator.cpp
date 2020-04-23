/**
 * @file model_estimator.cpp
 * @brief Model estimation by ORSA (aka AC-RANSAC) algorithm
 * @author Lionel Moisan, Pascal Monasse, Pierre Moulon
 *
 * Copyright (c) 2010-2011,2020 Pascal Monasse
 * Copyright (c) 2010-2011 Pierre Moulon
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "libOrsa/model_estimator.hpp"
#include "libOrsa/conditioning.hpp"

namespace orsa {

/// An isotropic normalization according to image dimensions is proposed, but
/// points remain unnormalized. Algorithms needing normalization for numerical
/// robustness must take care of it by themselves.
/// Matrices \a x1 and \a x2 are 2xn, representing Cartesian coordinates.
ModelEstimator::ModelEstimator(const Mat &x1, int w1, int h1,
                               const Mat &x2, int w2, int h2,
                               bool symmetricError)
: symError(symmetricError),
  x1_(x1), x2_(x2), N1_(3,3), N2_(3,3) {
  assert(2 == x1_.nrow());
  assert(x1_.nrow() == x2_.nrow());
  assert(x1_.ncol() == x2_.ncol());

  // Normalize both images by same factor, as our thresholds for SVD are based
  // on zoom around 1
  int w=std::max(w1,w2), h=std::max(h1,h2);
  PreconditionerFromPoints(w, h, &N1_);
  PreconditionerFromPoints(w, h, &N2_);
}

/// If multiple solutions are possible, return false.
bool ModelEstimator::ComputeModel(const std::vector<int> &indices,
                                  Model *model) const {
  std::vector<Model> models;
  Fit(indices, &models);
  if(models.size() != 1)
    return false;
  *model = models.front();
  return true;
}

} // namespace orsa

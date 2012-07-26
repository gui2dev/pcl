/*
 * Software License Agreement (BSD License)
 *
 *  Point Cloud Library (PCL) - www.pointclouds.org
 *  Copyright (c) 2012, Open Perception, Inc.
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 *
 */
#ifndef PCL_REGISTRATION_TRANSFORMATION_ESTIMATION_SVD_SCALE_HPP_
#define PCL_REGISTRATION_TRANSFORMATION_ESTIMATION_SVD_SCALE_HPP_

//////////////////////////////////////////////////////////////////////////////////////////////
template <typename PointSource, typename PointTarget> inline void
pcl::registration::TransformationEstimationSVDScale<PointSource, PointTarget>::estimateRigidTransformation (
    const pcl::PointCloud<PointSource> &cloud_src,
    const pcl::PointCloud<PointTarget> &cloud_tgt,
    Eigen::Matrix4f &transformation_matrix) const
{
  // <cloud_src,cloud_src> is the source dataset
  transformation_matrix.setIdentity ();

  Eigen::Vector4f centroid_src, centroid_tgt;
  // Estimate the centroids of source, target
  compute3DCentroid (cloud_src, centroid_src);
  compute3DCentroid (cloud_tgt, centroid_tgt);

  // Subtract the centroids from source, target
  Eigen::MatrixXf cloud_src_demean;
  demeanPointCloud (cloud_src, centroid_src, cloud_src_demean);

  Eigen::MatrixXf cloud_tgt_demean;
  demeanPointCloud (cloud_tgt, centroid_tgt, cloud_tgt_demean);

  getTransformationFromCorrelation (cloud_src_demean, centroid_src, cloud_tgt_demean, centroid_tgt, transformation_matrix);
}

//////////////////////////////////////////////////////////////////////////////////////////////
template <typename PointSource, typename PointTarget> void
pcl::registration::TransformationEstimationSVDScale<PointSource, PointTarget>::estimateRigidTransformation (
    const pcl::PointCloud<PointSource> &cloud_src,
    const std::vector<int> &indices_src,
    const pcl::PointCloud<PointTarget> &cloud_tgt,
    Eigen::Matrix4f &transformation_matrix) const
{
  if (indices_src.size () != cloud_tgt.points.size ())
  {
    PCL_ERROR ("[pcl::TransformationSVD::estimateRigidTransformation] Number or points in source (%zu) differs than target (%zu)!\n", indices_src.size (), cloud_tgt.points.size ());
    return;
  }

  // <cloud_src,cloud_src> is the source dataset
  transformation_matrix.setIdentity ();

  Eigen::Vector4f centroid_src, centroid_tgt;
  // Estimate the centroids of source, target
  compute3DCentroid (cloud_src, indices_src, centroid_src);
  compute3DCentroid (cloud_tgt, centroid_tgt);

  // Subtract the centroids from source, target
  Eigen::MatrixXf cloud_src_demean;
  demeanPointCloud (cloud_src, indices_src, centroid_src, cloud_src_demean);

  Eigen::MatrixXf cloud_tgt_demean;
  demeanPointCloud (cloud_tgt, centroid_tgt, cloud_tgt_demean);

  getTransformationFromCorrelation (cloud_src_demean, centroid_src, cloud_tgt_demean, centroid_tgt, transformation_matrix);
}


//////////////////////////////////////////////////////////////////////////////////////////////
template <typename PointSource, typename PointTarget> inline void
pcl::registration::TransformationEstimationSVDScale<PointSource, PointTarget>::estimateRigidTransformation (
    const pcl::PointCloud<PointSource> &cloud_src,
    const std::vector<int> &indices_src,
    const pcl::PointCloud<PointTarget> &cloud_tgt,
    const std::vector<int> &indices_tgt,
    Eigen::Matrix4f &transformation_matrix) const
{
  if (indices_src.size () != indices_tgt.size ())
  {
    PCL_ERROR ("[pcl::TransformationEstimationSVD::estimateRigidTransformation] Number or points in source (%zu) differs than target (%zu)!\n", indices_src.size (), indices_tgt.size ());
    return;
  }

  // <cloud_src,cloud_src> is the source dataset
  transformation_matrix.setIdentity ();

  Eigen::Vector4f centroid_src, centroid_tgt;
  // Estimate the centroids of source, target
  compute3DCentroid (cloud_src, indices_src, centroid_src);
  compute3DCentroid (cloud_tgt, indices_tgt, centroid_tgt);

  // Subtract the centroids from source, target
  Eigen::MatrixXf cloud_src_demean;
  demeanPointCloud (cloud_src, indices_src, centroid_src, cloud_src_demean);

  Eigen::MatrixXf cloud_tgt_demean;
  demeanPointCloud (cloud_tgt, indices_tgt, centroid_tgt, cloud_tgt_demean);

  getTransformationFromCorrelation (cloud_src_demean, centroid_src, cloud_tgt_demean, centroid_tgt, transformation_matrix);
}

//////////////////////////////////////////////////////////////////////////////////////////////
template <typename PointSource, typename PointTarget> void
pcl::registration::TransformationEstimationSVDScale<PointSource, PointTarget>::estimateRigidTransformation (
    const pcl::PointCloud<PointSource> &cloud_src,
    const pcl::PointCloud<PointTarget> &cloud_tgt,
    const pcl::Correspondences &correspondences,
    Eigen::Matrix4f &transformation_matrix) const
{
  std::vector<int> indices_src, indices_tgt;
  pcl::registration::getQueryIndices (correspondences, indices_src);
  pcl::registration::getMatchIndices (correspondences, indices_tgt);

  // <cloud_src,cloud_src> is the source dataset
  Eigen::Vector4f centroid_src, centroid_tgt;
  // Estimate the centroids of source, target
  compute3DCentroid (cloud_src, indices_src, centroid_src);
  compute3DCentroid (cloud_tgt, indices_tgt, centroid_tgt);

  // Subtract the centroids from source, target
  Eigen::MatrixXf cloud_src_demean;
  demeanPointCloud (cloud_src, indices_src, centroid_src, cloud_src_demean);

  Eigen::MatrixXf cloud_tgt_demean;
  demeanPointCloud (cloud_tgt, indices_tgt, centroid_tgt, cloud_tgt_demean);

  getTransformationFromCorrelation (cloud_src_demean, centroid_src, cloud_tgt_demean, centroid_tgt, transformation_matrix);
}

//////////////////////////////////////////////////////////////////////////////////////////////
template <typename PointSource, typename PointTarget> void
pcl::registration::TransformationEstimationSVDScale<PointSource, PointTarget>::getTransformationFromCorrelation (
    const Eigen::MatrixXf &cloud_src_demean,
    const Eigen::Vector4f &centroid_src,
    const Eigen::MatrixXf &cloud_tgt_demean,
    const Eigen::Vector4f &centroid_tgt,
    Eigen::Matrix4f &transformation_matrix) const
{
  transformation_matrix.setIdentity ();

  // Assemble the correlation matrix H = source * target'
  Eigen::Matrix3f H = (cloud_src_demean * cloud_tgt_demean.transpose ()).topLeftCorner<3, 3>();

  // Compute the Singular Value Decomposition
  Eigen::JacobiSVD<Eigen::Matrix3f> svd (H, Eigen::ComputeFullU | Eigen::ComputeFullV);
  Eigen::Matrix3f u = svd.matrixU ();
  Eigen::Matrix3f v = svd.matrixV ();

  // Compute R = V * U'
  if (u.determinant () * v.determinant () < 0)
  {
    for (int x = 0; x < 3; ++x)
      v (x, 2) *= -1;
  }

  Eigen::Matrix3f R = v * u.transpose ();

  // rotated cloud
  Eigen::MatrixXf src_ = R * cloud_src_demean;
  
  float scale1, scale2;
  double sum_ss = 0.0f, sum_tt = 0.0f, sum_tt_ = 0.0f;
  for (unsigned corrIdx = 0; corrIdx < cloud_src_demean.cols (); ++corrIdx)
  {
    sum_ss += cloud_src_demean (0, corrIdx) * cloud_src_demean (0, corrIdx);
    sum_ss += cloud_src_demean (1, corrIdx) * cloud_src_demean (1, corrIdx);
    sum_ss += cloud_src_demean (2, corrIdx) * cloud_src_demean (2, corrIdx);
    
    sum_tt += cloud_tgt_demean (0, corrIdx) * cloud_tgt_demean (0, corrIdx);
    sum_tt += cloud_tgt_demean (1, corrIdx) * cloud_tgt_demean (1, corrIdx);
    sum_tt += cloud_tgt_demean (2, corrIdx) * cloud_tgt_demean (2, corrIdx);
    
    sum_tt_ += cloud_tgt_demean (0, corrIdx) * src_ (0, corrIdx);
    sum_tt_ += cloud_tgt_demean (1, corrIdx) * src_ (1, corrIdx);
    sum_tt_ += cloud_tgt_demean (2, corrIdx) * src_ (2, corrIdx);
  }
  
  scale1 = sqrt (sum_tt / sum_ss);
  scale2 = sum_tt_ / sum_ss;
  /*
  //find residuals
  Eigen::Matrix3f R1, R2;
  R1 = scale1 * R;
  R2 = scale2 * R;
  
  Eigen::MatrixXf res1 = cloud_tgt_demean - R1 * cloud_src_demean;
  Eigen::MatrixXf res2 = cloud_tgt_demean - R2 * cloud_src_demean;
  
  float residual1 = (res1.transpose () * res1).trace ();
  float residual2 = (res2.transpose () * res2).trace ();
  
  std::cerr << scale1 << " : " << residual1 << " vs. " << scale2 << " : " << residual2 << std::endl;
  */
  float scale = scale2;
  transformation_matrix.topLeftCorner<3, 3> () = scale * R;
  const Eigen::Vector3f Rc (scale * R * centroid_src.head<3> ());
  transformation_matrix.block <3, 1> (0, 3) = centroid_tgt.head<3> () - Rc;
}

//#define PCL_INSTANTIATE_TransformationEstimationSVD(T,U) template class PCL_EXPORTS pcl::registration::TransformationEstimationSVD<T,U>;

#endif /* PCL_REGISTRATION_TRANSFORMATION_ESTIMATION_SVD_SCALE_HPP_ */

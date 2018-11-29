#ifndef rosdyn_par_estim_201811281028
#define rosdyn_par_estim_201811281028

#include <ros/ros.h>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Cholesky>
#include <Eigen/Dense>

#include <rosdyn_core/primitives.h>
#include <rosdyn_core/spacevect_algebra.h>

/*! \file meto_par_estim.h
    \brief A Documented file.
    
    Details.
*/

namespace rosdyn
{
 
  //! The base class for the dynamics parameters estimation. 
  /*!
    The class load log binary files (coming from the experiments) and estimates the dynamics parameters set (base parameters).
    The estimated parameters are saved into an URDF file.
  */
  class MetoParEstim
  {
  protected:
   
  
    //! NodeHandle pointer. 
    ros::NodeHandle                           m_nh;
    
    //! Matrix to convert the base parameter to full parameters vector considering zero null vector (pay attention: IF used as left hand side --> convert from the BASE to the FULL parameters set IF used as right hand side --> convert from the FULL to the BASE parameters set). 
    Eigen::MatrixXd                           m_T_base_to_full;
    
    //! Matrix from null to full. 
    Eigen::MatrixXd                           m_T_null_to_full;
    
    //! Base dynamics regression matrix. 
    Eigen::MatrixXd                           m_phi_base;
    
    //! Base dynamics regression matrix. 
    Eigen::MatrixXd                           m_phi_full;

    //! Base dynamics parameters vector.  
    Eigen::VectorXd                           m_base_par;
    
    //! Full dynamics parameters vector.  
    Eigen::VectorXd                           m_full_par;
    
    //! Pointer to Model Interface . 
    urdf::ModelInterfaceSharedPtr             m_model;
    
    //! Dynamics Regressor SVD decomposition.  
    Eigen::JacobiSVD<Eigen::MatrixXd>         m_svd_num_base_phi;
    
    
    Eigen::JacobiSVD<Eigen::MatrixXd>         m_svd_null;
    
    //! Additional components.  
    std::vector<rosdyn::ComponentPtr> m_components;
    
    //! Chain described into the URDF file. 
    boost::shared_ptr<rosdyn::Chain>  m_chain;
    
    //! Number of the additional paramters (usually friction parameters).  
    unsigned int                              m_additional_parameters;
    
    //! Robot name. 
    std::string                               m_robot_name;
    
    
    //! Names of the active joints. 
    std::vector<std::string>                  m_joints_names;
    
    //! Verbosity flag. 
    bool                                      m_verbose;
    
    //! Dynamcs parameters estimated flag.
    bool                                      m_par_computed;
    
    double m_condition_number;
    Eigen::VectorXd m_nominal_parameters;
    
    void loadNominalParameters();

  public:
    //! A constructor.
    /*!
      \param nh NodeHandle.
      \param xml_path path of the XML file (URDF).
      \param verbose enable the verbosity
    */
    MetoParEstim( ros::NodeHandle& nh, const std::string& robot_description, const bool& verbose = false );
    
    
    boost::shared_ptr<rosdyn::Chain> getChain(){return m_chain;};
    
    //! A constructor.
    /*!
      \param ptr_nh pointer to NodeHandle.
      \param xml_path path of the XML file (URDF).
      \param T
      \param verbose enable the verbosity
    */
    MetoParEstim( ros::NodeHandle& nh, 
                  const std::string& robot_description,
                  Eigen::MatrixXd& T, 
                  const bool& verbose = false );

    
    //! Get the base dynamics regression matrix for a given trajectory.
    /*!
      \param q joints positions along the trajectory.
      \param Dq joints velocities along the trajectory.
      \param DDq joints accelerations along the trajectory.
    */
    Eigen::MatrixXd           getTrajectoryRegressor( const Eigen::MatrixXd& q, 
                                                      const Eigen::MatrixXd& Dq, 
                                                      const Eigen::MatrixXd& DDq);
    
    Eigen::MatrixXd           getTrajectoryFullRegressor( const Eigen::MatrixXd& q, 
                                                      const Eigen::MatrixXd& Dq, 
                                                      const Eigen::MatrixXd& DDq);
    
    double getConditionNumber();
    //! Get the vector of the estimated base dynamics parameters.
    /*!
      \param tau joints torques along the trajectory.
    */
    Eigen::VectorXd           getEstimatedParameters(const Eigen::MatrixXd& tau);
    
     
    //! Get the vector of the estimated full dynamics parameters.
    /*!
      
    */
    Eigen::VectorXd           getFullEstimatedParameters();
    
    
    //! Get the robot name.
    /*!
      
    */
    std::string               getRobotName() { return m_robot_name; };
    
    
    //! Get the robot joints names.
    /*!
      
    */
    std::vector<std::string>  getRobotJointName() { return m_joints_names; };
    
    
    //! Save the estimated parameters into the XML file (URDF).
    /*!
      \param xml_save_path XML file path.
      \param add_info_save_path path of the additional information file.
      \param add_info_param_namespace additional information parameter namespace.
      \param dyn_par dynamics paramters vector.
      \param controlled_joints names of the controlled joints.
    */
    bool                      saveParXml( const std::string&              xml_save_path,
                                          const std::string&              add_info_save_path,
                                          const std::string&              add_info_param_namespace,
                                          const Eigen::VectorXd&          dyn_par,
                                          const std::vector<std::string>& controlled_joints  );
    
    
    
    
  };


}


#endif
#ifndef __METO_GEN_INTERFACE__
#define __METO_GEN_INTERFACE__

#include <mutex>
#include <thread>
#include <fstream> 
#include <iostream>
#include <boost/bind.hpp>

#include <ros/ros.h>
#include <nodelet/nodelet.h>
#include <actionlib/server/simple_action_server.h>

#include <rosdyn_identification_msgs/MetoTrjGenAction.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit_planning_helper/spline_interpolator.h>
#include <moveit_planning_helper/manage_trajectories.h>
#include <moveit/robot_model_loader/robot_model_loader.h>
#include <moveit/planning_scene/planning_scene.h>

#include <rosdyn_identification/rosdyn_par_estim.h>

/*! \file meto_gen_interface.h
    \brief A Documented file.
    
    Details.
*/

namespace rosdyn
{
  
  //! The Nodelet inteface class to \link MetoTrjOptGenTrj \endlink and to \link MetoTrjOptCheckCollision \endlink. 
  /*!
    The class is the Nodelet interface beetween the base class \link MetoTrjOptGenTrj \endlink and to the class \link MetoTrjOptCheckCollision \endlink.
    The class creates an Action Server fo the rosdyn_identification::MetoTrjGenAction to generates the  
  */
  class MetoGenInterfaceNodelet : public nodelet::Nodelet
  {
  public:
    
    //! Initialization of the ROS infrastructure for Nodelet.
    /*!
      Virtual and must be overridden by subclass. All initialization of the ROS infrastructure must be put into this function.
    */
    virtual void onInit();
    
  protected:
    double m_max_speed;
    double m_max_acc;
    
    //! Flag stop the Nodelet thread.
    bool m_stop;
    
    //! Node parameters' namepace.
    std::string m_namespace;
    
    //! Main Nodelet thread for the exciting trajectories generation.
    std::thread m_main_thread;
    
    
    //! Action Server.
    /*!
      Action Server of the rosdyn_identification::MetoTrjGenAction.
    */
    std::shared_ptr<actionlib::SimpleActionServer<rosdyn_identification_msgs::MetoTrjGenAction>> m_meto_trj_opt_as;
    
    std::shared_ptr<moveit::planning_interface::MoveGroupInterface> m_move_group;
    robot_model::RobotModelPtr m_kinematic_model;
    std::shared_ptr<planning_scene::PlanningScene> m_planning_scene;
    std::shared_ptr<rosdyn::MetoParEstim> m_meto_par_estim;
    collision_detection::CollisionRequest m_collision_request;
    unsigned int m_npoints_per_region;
    std::string m_trj_namespace;
    //! Main function to enter in an infinite loop.
    /*!
      
    */
    void main();
    
    
    //! Callback of the rosdyn_identification::MetoTrjGenAction.
    /*!
      \param goal goal of the rosdyn_identification::MetoTrjGenAction
    */
    void metoTrjGenCB( const rosdyn_identification_msgs::MetoTrjGenGoalConstPtr& goal );
    
    bool firstStageRandomTrajectory(ros::Duration duration, trajectory_msgs::JointTrajectory& trj);
    bool secondStageRandomTrajectory(const unsigned int& num_of_regions, trajectory_msgs::JointTrajectory& trj, std::vector< double >& initial_joint_conf);
    double checkConditionNumber(const trajectory_msgs::JointTrajectory& trj, double sampling_time);
    bool checkCollision(moveit::core::RobotState& state);
    //! A destructor of the class \link MetoGenInterfaceNodelet \endlink.
    /*!
    */
    ~MetoGenInterfaceNodelet();
  
    
  };

};


#endif
#include "IKinematics.hpp"
#include <iostream>
#include <numeric>      // std::inner_product

IKinematics::IKinematics()
{
leftLegFreeQVals.resize(kinLeftLeg::GetNumFreeParameters() );
rightLegFreeQVals.resize(kinRightLeg::GetNumFreeParameters() );

}


long IKinematics::LeftLegFwdKin(const std::vector<double>& q, std::vector<double> & pose)
{
    kinLeftLeg::ComputeFk(q.data(),&pose[0], &pose[3]);

    return 0;
}

long IKinematics::RightLegFwdKin(const std::vector<double>& q, std::vector<double> & pose)
{
    kinRightLeg::ComputeFk(q.data(),&pose[0], &pose[3]);

    return 0;
}

long IKinematics::LeftLegInvKin(const std::vector<double> & pose, std::vector<double>& q)
{


    kinLeftLeg::ComputeIk(&pose[0], &pose[3], 0, leftLegIKList);

    if (leftLegIKList.GetNumSolutions() < 1)
    {
        std::cout << "inverse kinematics failed for pose: " << std::endl;
        std::cout << "Position " << pose[0] << ", " << pose[1] << ", " << pose[2] << std::endl;
        std::cout << "Rotation " << std::endl;
        std::cout <<  pose[3] << ", " << pose[4] << ", " << pose[5] << std::endl;
        std::cout <<  pose[6] << ", " << pose[7] << ", " << pose[8] << std::endl;
        std::cout <<  pose[9] << ", " << pose[10] << ", " << pose[11] << std::endl;

    }

    long n = leftLegIKList.GetNumSolutions()-1;

    std::vector<double> mods(n);
    long dist=0;
    std::vector<double> sol(q.size());


    for (long i = 0; i< n; i++)
    {
        leftLegIKSol = &leftLegIKList.GetSolution(i);
        leftLegIKSol->GetSolution(sol,leftLegFreeQVals);
        mods[i] = std::inner_product(sol.begin(),sol.end(),sol.begin(),0);
    }

    dist=std::distance (  mods.begin() , std::min_element(mods.begin(),mods.end())  );
    leftLegIKSol = &leftLegIKList.GetSolution( dist );
    leftLegIKSol->GetSolution(q,leftLegFreeQVals);

    return 0;
}

long IKinematics::RightLegInvKin(const std::vector<double> & pose, std::vector<double>& q)
{


    kinRightLeg::ComputeIk(&pose[0], &pose[3], 0, rightLegIKList);

    long n = rightLegIKList.GetNumSolutions()-1;

    std::vector<double> mods(n);
    long dist=0;
    std::vector<double> sol(q.size());


    for (long i = 0; i< n; i++)
    {
        rightLegIKSol = &rightLegIKList.GetSolution(i);
        rightLegIKSol->GetSolution(sol,rightLegFreeQVals);
        mods[i] = std::inner_product(sol.begin(),sol.end(),sol.begin(),0);
    }

    dist=std::distance (  mods.begin() , std::min_element(mods.begin(),mods.end())  );
    rightLegIKSol = &rightLegIKList.GetSolution( dist );
    rightLegIKSol->GetSolution(q,rightLegFreeQVals);

    return 0;
}

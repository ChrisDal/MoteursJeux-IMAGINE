#pragma once
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>


/**
 * \file GeomPhysics.h
 * \brief Header file for general geometry concepts
 * 
 * \version 0.1
 *
 * Definition of struct in \a SpaceEngine namespace
*/

/**
 * \namespace SpaceEngine
 * \brief NameSpace for Engine Definition
 *
*/
namespace SpaceEngine 
{
	/**
	 * \struct Ray
	 * \brief A Ray implementation
	 * 
	 * Define a Ray with an origin and a direction
	*/
	struct Ray
	{
		glm::vec3 origin; /*!< Origin point of the Ray  */ 
		glm::vec3 direction; /*!<  Direction of the Ray */

		/**
		* Ray Default Constructor 
		*/
		inline Ray(): origin(0.0f, 0.0f, 0.0f), direction(0.0f, 0.0f, 1.0f){}; 

		/**
		* Ray Constructor taking origin and description
		*/
		inline Ray(const glm::vec3& point, const glm::vec3 dir) 
			: origin(point), direction(dir)
		{
			direction = glm::normalize(direction); 
		}

		/**
		* \brief  Create a Ray from a point \source in direction point \dest 
		* 
		* \param source : source point 
		* \param dest : destination point 
		* \return : Ray with normalized direction
		*/
		Ray fromPoints(const glm::vec3& source, const glm::vec3& dest)
		{
			return Ray(source, dest - source); 
		}
	};

	/**
	* \struct boundingBox 
	* \brief Bounding Box define with 3 points 
	* 
	*/
	struct boundingBox
	{
		glm::vec3 minbbox; /*!< Min point of the bouding box  */ 
		glm::vec3 maxbbox; /*!< Max point of the bounding box  */ 
		glm::vec3 center; /*!< Center of the bounding Box */ 

	};





}

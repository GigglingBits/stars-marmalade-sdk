using UnityEngine;
using System.Collections;

public class CameraAdjuster : MonoBehaviour
{

	// Use this for initialization
	void Start ()
	{
	
	}
	
	// Update is called once per frame
	void Update ()
	{
	
	}

	
	
	
	// <summary>Returns the size of the world in world units</summary> 
	private void InitializeCamera ()
	{
	}

	// <summary>Returns the size of the world in world units</summary> 
	private Vector2 GetWorldSize ()
	{
		return GameFoundation.instance.worldSize;
	}
	
	// <summary>Returns the size of the world in world units</summary> 
	private Rect GetView ()
	{
		Camera cam = Camera.main;
		float halfheight = cam.orthographicSize;
		float halfwidth = halfheight * cam.aspect;
		
		Vector3 center = cam.ViewportToWorldPoint (new Vector3 (0, 0, 0));
		
		return new Rect (center.x - halfwidth, center.y + halfheight, 2.0f * halfwidth, 2.0f * halfheight);
	}
	
	// <summary>Returns the size of the screen in pixels</summary> 
	private Vector2 GetScreenSize ()
	{
		return new Vector2 (Screen.width, Screen.height);
	}


}

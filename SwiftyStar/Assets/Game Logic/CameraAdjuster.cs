	using UnityEngine;
using System.Collections;
using System.Diagnostics;

public class CameraAdjuster : MonoBehaviour
{
	public float _transitionSpeed = 3.0f;

	// Use this for initialization
	void Start ()
	{
	}
	
	// Update is called once per frame
	void Update ()
	{
		if (_transitionSpeed < 0.0f) {
			_transitionSpeed = 0.0f;
		}
		if (_transitionSpeed > 10.0f) {
			_transitionSpeed = 10.0f;
		}

		float throttle = _transitionSpeed * Time.deltaTime;

		Rect vrect = GetViewCoords ();
		Rect wrect = GetWorldCoords ();

		// move camera towards world center (if offset > 1cm)
		Vector2 viewCenterOffset = 
			vrect.center - wrect.center;
		if (viewCenterOffset.magnitude > 0.01f) {
			Vector3 pos = GetCamera ().transform.position;
			pos.x -= viewCenterOffset.x * throttle;
			pos.y -= viewCenterOffset.y * throttle;
			GetCamera ().transform.position = pos;
		}

		// zoom to fit (if scale error > 1%)
		// only the y axis is relevant for zooming as this game is a side scroller
		float viewResizeFactorY = Mathf.Abs (wrect.height / vrect.height);
		if (viewResizeFactorY < 0.99f || viewResizeFactorY > 1.01) {
			float error = (Camera.main.orthographicSize * viewResizeFactorY) - Camera.main.orthographicSize;
			Camera.main.orthographicSize += error * throttle;
		}
	}

	private Camera GetCamera ()
	{
		return Camera.main;
	}

	// <summary>Returns the size of the world in world units</summary> 
	private Rect GetWorldCoords ()
	{
		Vector2 size = GameFoundation.instance.worldSize;
		return new Rect (0.0f, 0.0f, size.x, size.y);
	}
	
	// <summary>Returns the size of the view in world units</summary> 
	private Rect GetViewCoords ()
	{
		Camera cam = GetCamera ();
		float halfheight = cam.orthographicSize;
		float halfwidth = halfheight * cam.aspect;
		return new Rect (cam.transform.position.x - halfwidth, cam.transform.position.y + halfheight, 2.0f * halfwidth, 2.0f * -halfheight);
	}
	
	// <summary>Returns the size of the screen in pixels</summary> 
	private Rect GetScreenCoords ()
	{
		return new Rect (0.0f, 0.0f, Screen.width, Screen.height);
	}
	
	void OnDrawGizmos ()
	{
		DrawGizmoRect (GetWorldCoords (), Color.green);
		DrawGizmoRect (GetViewCoords (), Color.red);
	}

	private void DrawGizmoRect (Rect r, Color c)
	{
		Gizmos.color = c;
		Gizmos.DrawLine (new Vector3 (r.x, r.y, 0.0f), new Vector3 (r.x + r.width, r.y, 0.0f));
		Gizmos.DrawLine (new Vector3 (r.x + r.width, r.y, 0.0f), new Vector3 (r.x + r.width, r.y + r.height, 0.0f));
		Gizmos.DrawLine (new Vector3 (r.x + r.width, r.y + r.height, 0.0f), new Vector3 (r.x, r.y + r.height, 0.0f));
		Gizmos.DrawLine (new Vector3 (r.x, r.y + r.height, 0.0f), new Vector3 (r.x, r.y, 0.0f));

		Gizmos.DrawLine (r.center, Vector3.zero);
	}
}


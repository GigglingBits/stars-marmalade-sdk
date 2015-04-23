using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class CloudLogic : MonoBehaviour
{

	public float _cloudSpeedMetersPerSeconds = 1.0f; // m/s
	public float _cloudScale = 1.0f; 

	private Vector2 _cloudSize = Vector2.zero;

	public void Start ()
	{
		InitializeTransform ();
	}
	
	public void Update ()
	{
		Move ();
		ManageLyfecycle ();
	}

	private void InitializeTransform ()
	{
		ConfigureDistance ();

		Vector3 size = GetSizeWorldSpace ();
		_cloudSize.x = size.x;
		_cloudSize.y = size.y;

		ConfigureStartLocation (_cloudSize);
	}
	
	private void ConfigureDistance ()
	{
		float depth = Random.Range (0.133f, 1.0f);
		_cloudSpeedMetersPerSeconds *= depth;

		transform.localScale = new Vector3 (depth * _cloudScale, depth * _cloudScale, depth * _cloudScale);

		Vector3 pos = transform.localPosition;
		pos.z = -depth;
		transform.localPosition = pos;
	}

	Vector3 GetSizeWorldSpace ()
	{
		return gameObject.GetComponent<SpriteRenderer> ().bounds.size;
	}

	void ConfigureStartLocation (Vector2 cloudSize)
	{
		float worldwidth = GameFoundation.instance.worldSize.x;
		float worldheight = GameFoundation.instance.worldSize.y;

		Vector3 pos = transform.position;
		pos.x = worldwidth + cloudSize.x;
		pos.y = Random.Range (0.0f + cloudSize.y, worldheight - cloudSize.y);
		transform.position = pos;
	}	

	void Move ()
	{
		var pos = transform.localPosition;
		pos.x -= _cloudSpeedMetersPerSeconds * Time.deltaTime;
		transform.localPosition = pos;
	}

	void ManageLyfecycle ()
	{
		bool outofbounds = transform.position.x < -_cloudSize.x;
		if (outofbounds) {
			Destroy (gameObject);
		}
	}
}

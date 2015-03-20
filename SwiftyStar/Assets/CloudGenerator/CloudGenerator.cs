using UnityEngine;
using System.Collections;

public class CloudGenerator : MonoBehaviour {

	private const float CLOUD_INTERVAL_SEC = 3.0f; 
	private float countdown;

	public CloudGenerator () {
		countdown = CLOUD_INTERVAL_SEC;
	}

	public void Start () {

	}
	
	public void Update () {
		countdown -= Time.deltaTime;
		if (countdown <= 0.0f) {
			countdown += CLOUD_INTERVAL_SEC;
			CreateNextCloud();
		}
	}
	
	private void CreateNextCloud() {
		GameObject cube = GameObject.CreatePrimitive(PrimitiveType.Cube);
		cube.AddComponent<Rigidbody>();
		cube.transform.position = new Vector3(0, 0, 0);	

	//	GetComponents

		// create a cloud

	}
}

using UnityEngine;
using System.Collections;

public class JustSomeRotation : MonoBehaviour {

	void Start () {
	}
	
	void Update () {
		// just give it some rotation, so that we can see some movement on the screen
		transform.Rotate (Time.deltaTime * 20.0f, Time.deltaTime * 20.0f, Time.deltaTime * 20.0f);
	}
}

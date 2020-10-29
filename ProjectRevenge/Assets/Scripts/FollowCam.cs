using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FollowCam : MonoBehaviour
{
	[SerializeField]
	protected GameObject player;
	[SerializeField]
	protected float smoothTimeX, smoothTimeY;
	[SerializeField]
	protected Vector2 velocity;
	[SerializeField]
	protected float minX, maxX;

	private void Start()
	{
		//캐릭터 초기화
		player = GameObject.FindGameObjectWithTag("Player");
	}

	//캐릭터 위치에 따라 카메라가 이동하는 함수
	private void FixedUpdate()
	{
		//SmoothDamp는 천천히 값을 증가시키는 함수이다.
		float posX = Mathf.SmoothDamp(transform.position.x, player.transform.position.x, ref velocity.x, smoothTimeX);
		float posY = Mathf.SmoothDamp(transform.position.y, player.transform.position.y + 1.2f, ref velocity.y, smoothTimeY);

		//카메라로 이동
		transform.position = new Vector3(Mathf.Clamp(posX, minX, maxX), posY, transform.position.z);
	}
}

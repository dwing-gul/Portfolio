using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Nightmare : MonoBehaviour
{
    private Rigidbody2D mRigidbody2D;
    private Player player;
    public bool Ride;
    public float MoveSpeed;

    // Start is called before the first frame update
    void Start()
    {
        mRigidbody2D = GetComponent<Rigidbody2D>();
        player = GameObject.FindGameObjectWithTag("Player").GetComponent<Player>();
        Ride = false;
    }

    // Update is called once per frame
    void Update()
    {
        if (player.mbRide)
        {
            player.transform.SetParent(transform);
            player.gameObject.SetActive(false);
            Ride = true;

            UnityEngine.Debug.Log("horse");
        }
        else
        {
            player.transform.SetParent(null);
            player.gameObject.SetActive(true);
            Ride = false;
        }

        if (Ride)
        {
            mRigidbody2D.velocity = transform.right * MoveSpeed;
        }
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {

        if (collision.gameObject.CompareTag("EndPoint"))
        {
            player.mbRide = false;
        }
    }
}

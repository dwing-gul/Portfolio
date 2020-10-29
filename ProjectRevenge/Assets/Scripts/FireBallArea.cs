using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FireBallArea : MonoBehaviour
{
    private void OnTriggerExit2D(Collider2D collision)
    {
        if(collision.gameObject.CompareTag("FireBall"))
        {
            collision.gameObject.SetActive(false);
        }
    }
}

using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SpearAttackArea : MonoBehaviour
{
    public int Damage;

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if (collision.gameObject.CompareTag("Boss"))
        {
            collision.SendMessage("Hit", Damage, SendMessageOptions.DontRequireReceiver);
        }
    }
}

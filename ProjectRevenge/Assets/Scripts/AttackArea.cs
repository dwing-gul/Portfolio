using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AttackArea : MonoBehaviour
{
    private int Damage;

    public void SetDamage(int damage)
    {
        Damage = damage;
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        if(collision.gameObject.CompareTag("Enemy") | collision.gameObject.CompareTag("Boss") | collision.gameObject.CompareTag("HitTrigger"))
        {
            collision.SendMessage("Hit", Damage, SendMessageOptions.DontRequireReceiver);
        }
    }
}

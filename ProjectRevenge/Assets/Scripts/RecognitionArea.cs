using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RecognitionArea : MonoBehaviour
{
    [SerializeField]
    private Wizard Owner;
    private bool isEnter;

    private void OnTriggerStay2D(Collider2D collision)
    {
        if (collision.gameObject.CompareTag("Player"))
        {
            isEnter = true;
            Owner.LongDistanceAttack(isEnter);
            UnityEngine.Debug.Log("PlayerEnter");
        }
    }

    private void OnTriggerExit2D(Collider2D collision)
    {
        if (collision.gameObject.CompareTag("Player"))
        {
            isEnter = false;
            Owner.LongDistanceAttack(isEnter);
        }
    }
}
